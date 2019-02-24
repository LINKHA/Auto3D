#include "Image.h"
#define AUTO_IMAGE_WRITE_IMPL
#include "AutoImage.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "AutoOGL.h"
#include "Decompress.h"

#include "NewDef.h"

namespace Auto3D {


const size_t Image::pixelByteSizes[] =
{
	0,      // FMT_NONE
	1,      // FMT_R8
	2,      // FMT_RG8
	4,      // FMT_RGBA8
	1,      // FMT_A8
	2,      // FMT_R16
	4,      // FMT_RG16
	8,      // FMT_RGBA16
	2,      // FMT_R16F
	4,      // FMT_RG16F
	8,      // FMT_RGBA16F
	4,      // FMT_R32F
	8,      // FMT_RG32F
	12,     // FMT_RGB32F
	16,     // FMT_RGBA32F
	2,      // FMT_D16
	4,      // FMT_D32
	4,      // FMT_D24S8
	0,      // FMT_DXT1
	0,      // FMT_DXT3
	0,      // FMT_DXT5
	0,      // FMT_ETC1
	0,      // FMT_PVRTC_RGB_2BPP
	0,      // FMT_PVRTC_RGBA_2BPP
	0,      // FMT_PVRTC_RGB_4BPP
	0       // FMT_PVRTC_RGBA_4BPP
};


bool CompressedLevel::Decompress(unsigned char* dest)
{
	if (!_data)
		return false;

	switch (_format)
	{
	case CompressedFormat::DXT1:
	case CompressedFormat::DXT3:
	case CompressedFormat::DXT5:
		DecompressImageDXT(dest, _data, _width, _height, _depth, _format);
		return true;

	case CompressedFormat::ETC1:
		DecompressImageETC(dest, _data, _width, _height);
		return true;

	case CompressedFormat::PvrtcRGB2bpp:
	case CompressedFormat::PvrtcRGBA2bpp:
	case CompressedFormat::PvrtcRGB4bpp:
	case CompressedFormat::PvrtcRGBA4bpp:
		DecompressImagePVRTC(dest, _data, _width, _height, _format);
		return true;

	default:
		// Unknown format
		return false;
	}
}

Image::Image(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}

Image::~Image()
{
}

void Image::RegisterObject(SharedPtr<Ambient> ambient)
{
	ambient->RegisterFactory<Image>();
}

bool Image::BeginLoad(Deserializer& source)
{
	source.Seek(0);
	int width, height;
	unsigned components;
	unsigned char* pixelData = getImageData(source, width, height, components);
	if (!pixelData)
	{
		AutoCout << "Could not load image " + source.GetName() << ": " << STRING(stbi_failure_reason()) << AutoEndl;
		return false;
	}
	SetFormat(components);
	SetSize(width, height, components);
	SetData(pixelData);
	freeImageData(pixelData);

	return true;
}

bool Image::Save(Serializer& dest) const
{

	if (IsCompressed())
	{
		WarningString("Can not save compressed image " + GetName());
		return false;
	}

	if (!_data)
	{
		WarningString("Can not save zero-sized image " + GetName());
		return false;
	}

	int len;
	unsigned char* png = stbi_write_png_to_mem(_data.get(), 0, _width, _height, _components, &len);
	bool success = dest.Write(png, (unsigned)len) == (unsigned)len;
	free(png);      // NOLINT(hicpp-no-malloc)
	return success;
}

bool Image::SetSize(int width, int height, unsigned comp)
{
	return SetSize(width, height, 1, comp);
}

bool Image::SetSize(int width, int height, int depth, unsigned comp)
{
	if (width == _width && height == _height && depth == _depth && comp == _components)
		return true;

	if (width <= 0 || height <= 0 || depth <= 0)
		return false;

	if (comp > 4)
	{
		WarningString("More than 4 color components are not supported");
		return false;
	}

	_data = SharedArrayPtr<unsigned char>(new unsigned char[width * height * depth * comp]);
	_width = width;
	_height = height;
	_depth = depth;
	_components = comp;
	_compressedFormat = CompressedFormat::None;
	_numCompressedLevels = 0;
	_nextLevel.reset();

	SetMemoryUse(width * height * depth * comp);
	return true;
}

void Image::SetData(const unsigned char* pixelData)
{
	if (!_data)
		return;

	if (IsCompressed())
	{
		WarningString("Can not set new pixel data for a compressed image");
		return;
	}

	auto size = (size_t)_width * _height * _depth * _components;
	if (pixelData)
		memcpy(_data.get(), pixelData, size);
	else
		memset(_data.get(), 0, size);
	_nextLevel.reset();
}

void Image::SetFormat(unsigned comp)
{
	if (!comp)
		return;
	switch (comp)
	{
	case 1: _format = GL_RED; break;
	case 3: _format = GL_RGB; break;
	case 4: _format = GL_RGBA; break;
	default:
		break;
	}
}

void Image::SetImageType(ImageType type)
{
	_imageType = type;
}

unsigned char* Image::getImageData(Deserializer& source, int& width, int& height, unsigned& components)
{
	unsigned dataSize = source.GetSize();

	SharedArrayPtr<unsigned char> buffer(new unsigned char[dataSize]);
	source.Read(buffer.get(), dataSize);
	return stbi_load_from_memory(buffer.get(), dataSize, &width, &height, (int*)&components, 0);
}

SharedPtr<Image> Image::ConvertToRGBA() const
{
	if (IsCompressed())
	{
		LogString("Can not convert compressed image to RGBA");
		return SharedPtr<Image>();
	}
	if (_components < 1 || _components > 4)
	{
		LogString("Illegal number of image components for conversion to RGBA");
		return SharedPtr<Image>();
	}
	if (!_data)
	{
		LogString("Can not convert image without data to RGBA");
		return SharedPtr<Image>();
	}

	// Already RGBA?
	if (_components == 4)
		return SharedPtr<Image>(const_cast<Image*>(this));

	SharedPtr<Image> ret = MakeShared<Image>(_ambient);
	ret->SetSize(_width, _height, _depth, 4);

	const unsigned char* src = _data.get();
	unsigned char* dest = ret->GetData().get();

	switch (_components)
	{
	case 1:
		for (unsigned i = 0; i < static_cast<unsigned>(_width * _height * _depth); ++i)
		{
			unsigned char pixel = *src++;
			*dest++ = pixel;
			*dest++ = pixel;
			*dest++ = pixel;
			*dest++ = 255;
		}
		break;

	case 2:
		for (unsigned i = 0; i < static_cast<unsigned>(_width * _height * _depth); ++i)
		{
			unsigned char pixel = *src++;
			*dest++ = pixel;
			*dest++ = pixel;
			*dest++ = pixel;
			*dest++ = *src++;
		}
		break;

	case 3:
		for (unsigned i = 0; i < static_cast<unsigned>(_width * _height * _depth); ++i)
		{
			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = *src++;
			*dest++ = 255;
		}
		break;

	default:
		assert(false);  // Should never reach nere
		break;
	}

	return ret;
}

void Image::freeImageData(unsigned char* pixelData)
{
	if (!pixelData)
		return;

	stbi_image_free(pixelData);
}
SharedPtr<Image> Image::GetNextLevel() const
{
	if (IsCompressed())
	{
		LogString("Can not generate mip level from compressed data");
		return SharedPtr<Image>();
	}
	if (_components < 1 || _components > 4)
	{
		LogString("Illegal number of image components for mip level generation");
		return SharedPtr<Image>();
	}

	if (_nextLevel)
		return _nextLevel;

	int widthOut = _width / 2;
	int heightOut = _height / 2;
	int depthOut = _depth / 2;

	if (widthOut < 1)
		widthOut = 1;
	if (heightOut < 1)
		heightOut = 1;
	if (depthOut < 1)
		depthOut = 1;

	SharedPtr<Image> mipImage = MakeShared<Image>(_ambient);

	if (_depth > 1)
		mipImage->SetSize(widthOut, heightOut, depthOut, _components);
	else
		mipImage->SetSize(widthOut, heightOut, _components);

	const unsigned char* pixelDataIn = _data.get();
	unsigned char* pixelDataOut = mipImage->_data.get();

	// 1D case
	if (_depth == 1 && (_height == 1 || _width == 1))
	{
		// Loop using the larger dimension
		if (widthOut < heightOut)
			widthOut = heightOut;

		switch (_components)
		{
		case 1:
			for (int x = 0; x < widthOut; ++x)
				pixelDataOut[x] = (unsigned char)(((unsigned)pixelDataIn[x * 2] + pixelDataIn[x * 2 + 1]) >> 1);
			break;

		case 2:
			for (int x = 0; x < widthOut * 2; x += 2)
			{
				pixelDataOut[x] = (unsigned char)(((unsigned)pixelDataIn[x * 2] + pixelDataIn[x * 2 + 2]) >> 1);
				pixelDataOut[x + 1] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 1] + pixelDataIn[x * 2 + 3]) >> 1);
			}
			break;

		case 3:
			for (int x = 0; x < widthOut * 3; x += 3)
			{
				pixelDataOut[x] = (unsigned char)(((unsigned)pixelDataIn[x * 2] + pixelDataIn[x * 2 + 3]) >> 1);
				pixelDataOut[x + 1] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 1] + pixelDataIn[x * 2 + 4]) >> 1);
				pixelDataOut[x + 2] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 2] + pixelDataIn[x * 2 + 5]) >> 1);
			}
			break;

		case 4:
			for (int x = 0; x < widthOut * 4; x += 4)
			{
				pixelDataOut[x] = (unsigned char)(((unsigned)pixelDataIn[x * 2] + pixelDataIn[x * 2 + 4]) >> 1);
				pixelDataOut[x + 1] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 1] + pixelDataIn[x * 2 + 5]) >> 1);
				pixelDataOut[x + 2] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 2] + pixelDataIn[x * 2 + 6]) >> 1);
				pixelDataOut[x + 3] = (unsigned char)(((unsigned)pixelDataIn[x * 2 + 3] + pixelDataIn[x * 2 + 7]) >> 1);
			}
			break;

		default:
			assert(false);  // Should never reach here
			break;
		}
	}
	// 2D case
	else if (_depth == 1)
	{
		switch (_components)
		{
		case 1:
			for (int y = 0; y < heightOut; ++y)
			{
				const unsigned char* inUpper = &pixelDataIn[(y * 2) * _width];
				const unsigned char* inLower = &pixelDataIn[(y * 2 + 1) * _width];
				unsigned char* out = &pixelDataOut[y * widthOut];

				for (int x = 0; x < widthOut; ++x)
				{
					out[x] = (unsigned char)(((unsigned)inUpper[x * 2] + inUpper[x * 2 + 1] +
						inLower[x * 2] + inLower[x * 2 + 1]) >> 2);
				}
			}
			break;

		case 2:
			for (int y = 0; y < heightOut; ++y)
			{
				const unsigned char* inUpper = &pixelDataIn[(y * 2) * _width * 2];
				const unsigned char* inLower = &pixelDataIn[(y * 2 + 1) * _width * 2];
				unsigned char* out = &pixelDataOut[y * widthOut * 2];

				for (int x = 0; x < widthOut * 2; x += 2)
				{
					out[x] = (unsigned char)(((unsigned)inUpper[x * 2] + inUpper[x * 2 + 2] +
						inLower[x * 2] + inLower[x * 2 + 2]) >> 2);
					out[x + 1] = (unsigned char)(((unsigned)inUpper[x * 2 + 1] + inUpper[x * 2 + 3] +
						inLower[x * 2 + 1] + inLower[x * 2 + 3]) >> 2);
				}
			}
			break;

		case 3:
			for (int y = 0; y < heightOut; ++y)
			{
				const unsigned char* inUpper = &pixelDataIn[(y * 2) * _width * 3];
				const unsigned char* inLower = &pixelDataIn[(y * 2 + 1) * _width * 3];
				unsigned char* out = &pixelDataOut[y * widthOut * 3];

				for (int x = 0; x < widthOut * 3; x += 3)
				{
					out[x] = (unsigned char)(((unsigned)inUpper[x * 2] + inUpper[x * 2 + 3] +
						inLower[x * 2] + inLower[x * 2 + 3]) >> 2);
					out[x + 1] = (unsigned char)(((unsigned)inUpper[x * 2 + 1] + inUpper[x * 2 + 4] +
						inLower[x * 2 + 1] + inLower[x * 2 + 4]) >> 2);
					out[x + 2] = (unsigned char)(((unsigned)inUpper[x * 2 + 2] + inUpper[x * 2 + 5] +
						inLower[x * 2 + 2] + inLower[x * 2 + 5]) >> 2);
				}
			}
			break;

		case 4:
			for (int y = 0; y < heightOut; ++y)
			{
				const unsigned char* inUpper = &pixelDataIn[(y * 2) * _width * 4];
				const unsigned char* inLower = &pixelDataIn[(y * 2 + 1) * _width * 4];
				unsigned char* out = &pixelDataOut[y * widthOut * 4];

				for (int x = 0; x < widthOut * 4; x += 4)
				{
					out[x] = (unsigned char)(((unsigned)inUpper[x * 2] + inUpper[x * 2 + 4] +
						inLower[x * 2] + inLower[x * 2 + 4]) >> 2);
					out[x + 1] = (unsigned char)(((unsigned)inUpper[x * 2 + 1] + inUpper[x * 2 + 5] +
						inLower[x * 2 + 1] + inLower[x * 2 + 5]) >> 2);
					out[x + 2] = (unsigned char)(((unsigned)inUpper[x * 2 + 2] + inUpper[x * 2 + 6] +
						inLower[x * 2 + 2] + inLower[x * 2 + 6]) >> 2);
					out[x + 3] = (unsigned char)(((unsigned)inUpper[x * 2 + 3] + inUpper[x * 2 + 7] +
						inLower[x * 2 + 3] + inLower[x * 2 + 7]) >> 2);
				}
			}
			break;

		default:
			assert(false);  // Should never reach here
			break;
		}
	}
	// 3D case
	else
	{
		switch (_components)
		{
		case 1:
			for (int z = 0; z < depthOut; ++z)
			{
				const unsigned char* inOuter = &pixelDataIn[(z * 2) * _width * _height];
				const unsigned char* inInner = &pixelDataIn[(z * 2 + 1) * _width * _height];

				for (int y = 0; y < heightOut; ++y)
				{
					const unsigned char* inOuterUpper = &inOuter[(y * 2) * _width];
					const unsigned char* inOuterLower = &inOuter[(y * 2 + 1) * _width];
					const unsigned char* inInnerUpper = &inInner[(y * 2) * _width];
					const unsigned char* inInnerLower = &inInner[(y * 2 + 1) * _width];
					unsigned char* out = &pixelDataOut[z * widthOut * heightOut + y * widthOut];

					for (int x = 0; x < widthOut; ++x)
					{
						out[x] = (unsigned char)(((unsigned)inOuterUpper[x * 2] + inOuterUpper[x * 2 + 1] +
							inOuterLower[x * 2] + inOuterLower[x * 2 + 1] +
							inInnerUpper[x * 2] + inInnerUpper[x * 2 + 1] +
							inInnerLower[x * 2] + inInnerLower[x * 2 + 1]) >> 3);
					}
				}
			}
			break;

		case 2:
			for (int z = 0; z < depthOut; ++z)
			{
				const unsigned char* inOuter = &pixelDataIn[(z * 2) * _width * _height * 2];
				const unsigned char* inInner = &pixelDataIn[(z * 2 + 1) * _width * _height * 2];

				for (int y = 0; y < heightOut; ++y)
				{
					const unsigned char* inOuterUpper = &inOuter[(y * 2) * _width * 2];
					const unsigned char* inOuterLower = &inOuter[(y * 2 + 1) * _width * 2];
					const unsigned char* inInnerUpper = &inInner[(y * 2) * _width * 2];
					const unsigned char* inInnerLower = &inInner[(y * 2 + 1) * _width * 2];
					unsigned char* out = &pixelDataOut[z * widthOut * heightOut * 2 + y * widthOut * 2];

					for (int x = 0; x < widthOut * 2; x += 2)
					{
						out[x] = (unsigned char)(((unsigned)inOuterUpper[x * 2] + inOuterUpper[x * 2 + 2] +
							inOuterLower[x * 2] + inOuterLower[x * 2 + 2] +
							inInnerUpper[x * 2] + inInnerUpper[x * 2 + 2] +
							inInnerLower[x * 2] + inInnerLower[x * 2 + 2]) >> 3);
						out[x + 1] = (unsigned char)(((unsigned)inOuterUpper[x * 2 + 1] + inOuterUpper[x * 2 + 3] +
							inOuterLower[x * 2 + 1] + inOuterLower[x * 2 + 3] +
							inInnerUpper[x * 2 + 1] + inInnerUpper[x * 2 + 3] +
							inInnerLower[x * 2 + 1] + inInnerLower[x * 2 + 3]) >> 3);
					}
				}
			}
			break;

		case 3:
			for (int z = 0; z < depthOut; ++z)
			{
				const unsigned char* inOuter = &pixelDataIn[(z * 2) * _width * _height * 3];
				const unsigned char* inInner = &pixelDataIn[(z * 2 + 1) * _width * _height * 3];

				for (int y = 0; y < heightOut; ++y)
				{
					const unsigned char* inOuterUpper = &inOuter[(y * 2) * _width * 3];
					const unsigned char* inOuterLower = &inOuter[(y * 2 + 1) * _width * 3];
					const unsigned char* inInnerUpper = &inInner[(y * 2) * _width * 3];
					const unsigned char* inInnerLower = &inInner[(y * 2 + 1) * _width * 3];
					unsigned char* out = &pixelDataOut[z * widthOut * heightOut * 3 + y * widthOut * 3];

					for (int x = 0; x < widthOut * 3; x += 3)
					{
						out[x] = (unsigned char)(((unsigned)inOuterUpper[x * 2] + inOuterUpper[x * 2 + 3] +
							inOuterLower[x * 2] + inOuterLower[x * 2 + 3] +
							inInnerUpper[x * 2] + inInnerUpper[x * 2 + 3] +
							inInnerLower[x * 2] + inInnerLower[x * 2 + 3]) >> 3);
						out[x + 1] = (unsigned char)(((unsigned)inOuterUpper[x * 2 + 1] + inOuterUpper[x * 2 + 4] +
							inOuterLower[x * 2 + 1] + inOuterLower[x * 2 + 4] +
							inInnerUpper[x * 2 + 1] + inInnerUpper[x * 2 + 4] +
							inInnerLower[x * 2 + 1] + inInnerLower[x * 2 + 4]) >> 3);
						out[x + 2] = (unsigned char)(((unsigned)inOuterUpper[x * 2 + 2] + inOuterUpper[x * 2 + 5] +
							inOuterLower[x * 2 + 2] + inOuterLower[x * 2 + 5] +
							inInnerUpper[x * 2 + 2] + inInnerUpper[x * 2 + 5] +
							inInnerLower[x * 2 + 2] + inInnerLower[x * 2 + 5]) >> 3);
					}
				}
			}
			break;

		case 4:
			for (int z = 0; z < depthOut; ++z)
			{
				const unsigned char* inOuter = &pixelDataIn[(z * 2) * _width * _height * 4];
				const unsigned char* inInner = &pixelDataIn[(z * 2 + 1) * _width * _height * 4];

				for (int y = 0; y < heightOut; ++y)
				{
					const unsigned char* inOuterUpper = &inOuter[(y * 2) * _width * 4];
					const unsigned char* inOuterLower = &inOuter[(y * 2 + 1) * _width * 4];
					const unsigned char* inInnerUpper = &inInner[(y * 2) * _width * 4];
					const unsigned char* inInnerLower = &inInner[(y * 2 + 1) * _width * 4];
					unsigned char* out = &pixelDataOut[z * widthOut * heightOut * 4 + y * widthOut * 4];

					for (int x = 0; x < widthOut * 4; x += 4)
					{
						out[x] = (unsigned char)(((unsigned)inOuterUpper[x * 2] + inOuterUpper[x * 2 + 4] +
							inOuterLower[x * 2] + inOuterLower[x * 2 + 4] +
							inInnerUpper[x * 2] + inInnerUpper[x * 2 + 4] +
							inInnerLower[x * 2] + inInnerLower[x * 2 + 4]) >> 3);
						out[x + 1] = (unsigned char)(((unsigned)inOuterUpper[x * 2 + 1] + inOuterUpper[x * 2 + 5] +
							inOuterLower[x * 2 + 1] + inOuterLower[x * 2 + 5] +
							inInnerUpper[x * 2 + 1] + inInnerUpper[x * 2 + 5] +
							inInnerLower[x * 2 + 1] + inInnerLower[x * 2 + 5]) >> 3);
						out[x + 2] = (unsigned char)(((unsigned)inOuterUpper[x * 2 + 2] + inOuterUpper[x * 2 + 6] +
							inOuterLower[x * 2 + 2] + inOuterLower[x * 2 + 6] +
							inInnerUpper[x * 2 + 2] + inInnerUpper[x * 2 + 6] +
							inInnerLower[x * 2 + 2] + inInnerLower[x * 2 + 6]) >> 3);
					}
				}
			}
			break;

		default:
			assert(false);  // Should never reach here
			break;
		}
	}

	return mipImage;
}

CompressedLevel Image::GetCompressedLevel(unsigned index) const
{
	CompressedLevel level;

	if (_compressedFormat == CompressedFormat::None)
	{
		WarningString("Image is not compressed");
		return level;
	}
	if (index >= _numCompressedLevels)
	{
		WarningString("Compressed image mip level out of bounds");
		return level;
	}

	level._format = _compressedFormat;
	level._width = _width;
	level._height = _height;
	level._depth = _depth;

	if (_compressedFormat == CompressedFormat::RGBA)
	{
		level._blockSize = 4;
		unsigned i = 0;
		unsigned offset = 0;

		for (;;)
		{
			if (!level._width)
				level._width = 1;
			if (!level._height)
				level._height = 1;
			if (!level._depth)
				level._depth = 1;

			level._rowSize = level._width * level._blockSize;
			level._rows = (unsigned)level._height;
			level._data = _data.get() + offset;
			level._dataSize = level._depth * level._rows * level._rowSize;

			if (offset + level._dataSize > GetMemoryUse())
			{
				WarningString("Compressed level is outside image data. Offset: " + STRING(offset) + " Size: " + STRING(level._dataSize) +
					" Datasize: " + STRING(GetMemoryUse()));
				level._data = nullptr;
				return level;
			}

			if (i == index)
				return level;

			offset += level._dataSize;
			level._width /= 2;
			level._height /= 2;
			level._depth /= 2;
			++i;
		}
	}
	else if (_compressedFormat < CompressedFormat::PvrtcRGB2bpp)
	{
		level._blockSize = (_compressedFormat == CompressedFormat::DXT1 || _compressedFormat == CompressedFormat::ETC1) ? 8 : 16;
		unsigned i = 0;
		unsigned offset = 0;

		for (;;)
		{
			if (!level._width)
				level._width = 1;
			if (!level._height)
				level._height = 1;
			if (!level._depth)
				level._depth = 1;

			level._rowSize = ((level._width + 3) / 4) * level._blockSize;
			level._rows = (unsigned)((level._height + 3) / 4);
			level._data = _data.get() + offset;
			level._dataSize = level._depth * level._rows * level._rowSize;

			if (offset + level._dataSize > GetMemoryUse())
			{
				WarningString("Compressed level is outside image data. Offset: " + STRING(offset) + " Size: " + STRING(level._dataSize) +
					" Datasize: " + STRING(GetMemoryUse()));
				level._data = nullptr;
				return level;
			}

			if (i == index)
				return level;

			offset += level._dataSize;
			level._width /= 2;
			level._height /= 2;
			level._depth /= 2;
			++i;
		}
	}
	else
	{
		level._blockSize = _compressedFormat < CompressedFormat::PvrtcRGB4bpp ? 2 : 4;
		unsigned i = 0;
		unsigned offset = 0;

		for (;;)
		{
			if (!level._width)
				level._width = 1;
			if (!level._height)
				level._height = 1;

			int dataWidth = Max(level._width, level._blockSize == 2 ? 16 : 8);
			int dataHeight = Max(level._height, 8);
			level._data = _data.get() + offset;
			level._dataSize = (dataWidth * dataHeight * level._blockSize + 7) >> 3;
			level._rows = (unsigned)dataHeight;
			level._rowSize = level._dataSize / level._rows;

			if (offset + level._dataSize > GetMemoryUse())
			{
				WarningString("Compressed level is outside image data. Offset: " + STRING(offset) + " Size: " + STRING(level._dataSize) +
					" Datasize: " + STRING(GetMemoryUse()));
				level._data = nullptr;
				return level;
			}

			if (i == index)
				return level;

			offset += level._dataSize;
			level._width /= 2;
			level._height /= 2;
			++i;
		}
	}
}

size_t Image::CalculateDataSize(const Vector2I& size, ImageFormat format, size_t* dstRows, size_t* dstRowSize)
{
	size_t rows, rowSize, dataSize;

	if (format < ImageFormat::DXT1)
	{
		rows = size._y;
		rowSize = size._x * pixelByteSizes[(int)format];
		dataSize = rows * rowSize;
	}
	else if (format < ImageFormat::PVRTC_RGB_2BPP)
	{
		size_t blockSize = (format == ImageFormat::DXT1 || format == ImageFormat::ETC1) ? 8 : 16;
		rows = (size._y + 3) / 4;
		rowSize = ((size._x + 3) / 4) * blockSize;
		dataSize = rows * rowSize;
	}
	else
	{
		size_t blockSize = format < ImageFormat::PVRTC_RGB_4BPP ? 2 : 4;
		size_t dataWidth = Max(size._x, blockSize == 2 ? 16 : 8);
		rows = Max(size._y, 8);
		dataSize = (dataWidth * rows * blockSize + 7) >> 3;
		rowSize = dataSize / rows;
	}

	if (dstRows)
		*dstRows = rows;
	if (dstRowSize)
		*dstRowSize = rowSize;
	return dataSize;
}


}