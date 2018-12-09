#include "Image.h"
#include "Ambient.h"
#define AUTO_IMAGE_WRITE_IMPL
#include "AutoImage.h"
#include "Deserializer.h"
#include "Serializer.h"
#include "AutoOGL.h"
#include "NewDef.h"

namespace Auto3D {


Image::Image(Ambient* ambient)
	:Super(ambient)
{
}

Image::~Image() = default;

void Image::RegisterObject(Ambient* ambient)
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
	unsigned char* png = stbi_write_png_to_mem(_data, 0, _width, _height, _components, &len);
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

	_data = new unsigned char[width * height * depth * comp];
	_width = width;
	_height = height;
	_depth = depth;
	_components = comp;
	_compressedFormat = CompressedFormat::NONE;
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
		memcpy(_data, pixelData, size);
	else
		memset(_data, 0, size);
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
	stbi_set_flip_vertically_on_load(true);
	unsigned dataSize = source.GetSize();

	SharedArrayPtr<unsigned char> buffer(new unsigned char[dataSize]);
	source.Read(buffer.get(), dataSize);
	return stbi_load_from_memory(buffer.get(), dataSize, &width, &height, (int*)&components, 0);
}

void Image::freeImageData(unsigned char* pixelData)
{
	if (!pixelData)
		return;

	stbi_image_free(pixelData);
}

}