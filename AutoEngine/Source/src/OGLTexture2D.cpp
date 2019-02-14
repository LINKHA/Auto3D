#include "Texture2D.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "RenderSurface.h"
#include "Renderer.h"

namespace Auto3D {

Texture2D::Texture2D(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	_target = GL_TEXTURE_2D;
}

bool Texture2D::SetSize(int width, int height, unsigned format, TextureUsage usage, int multiSample, bool autoResolve)
{
	if (width <= 0 || height <= 0)
	{
		LogString("Zero or negative texture dimensions");
		return false;
	}

	multiSample = clamp(multiSample, 1, 16);
	if (multiSample == 1)
		autoResolve = false;
	else if (multiSample > 1 && usage < TextureUsage::RenderTarget)
	{
		LogString("Multisampling is only supported for rendertarget or depth-stencil textures");
		return false;
	}

	// Disable mipmaps if multisample & custom resolve
	if (multiSample > 1 && autoResolve == false)
		_requestedLevels = 1;

	_width = width;
	_height = height;
	_format = format;
	_depth = 1;
	_multiSample = multiSample;
	autoResolve = autoResolve;

	return create();
}

bool Texture2D::SetData(unsigned level, int x, int y, int width, int height, const void* data)
{

	if (!_object.name || !_graphics.lock())
	{
		WarningString("No texture created, can not set data");
		return false;
	}

	if (!data)
	{
		WarningString("Null source for setting data");
		return false;
	}

	if (level >= _levels)
	{
		WarningString("Illegal mip level for setting data");
		return false;
	}

	if (_graphics.lock()->IsDeviceLost())
	{
		LogString("Texture data assignment while device is lost");
		_dataPending = true;
		return true;
	}

	if (IsCompressed())
	{
		x &= ~3u;
		y &= ~3u;
	}

	int levelWidth = GetLevelWidth(level);
	int levelHeight = GetLevelHeight(level);
	if (x < 0 || x + width > levelWidth || y < 0 || y + height > levelHeight || width <= 0 || height <= 0)
	{
		LogString("Illegal dimensions for setting data");
		return false;
	}

	_graphics.lock()->SetTextureForUpdate(SharedFromThis(Texture2D));

	bool wholeLevel = x == 0 && y == 0 && width == levelWidth && height == levelHeight;
	unsigned format = _format;

	if (!IsCompressed())
	{
		if (wholeLevel)
			glTexImage2D(_target, level, format, width, height, 0, _format, GetDataType(_format), data);
		else
			glTexSubImage2D(_target, level, x, y, width, height, _format, GetDataType(_format), data);
	}

	else
	{
		if (wholeLevel)
			glCompressedTexImage2D(_target, level, format, width, height, 0, GetDataSize(width, height), data);
		else
			glCompressedTexSubImage2D(_target, level, x, y, width, height, format, GetDataSize(width, height), data);
	}

	_graphics.lock()->SetTexture(0, nullptr);
	return true;
}
bool Texture2D::SetData(SharedPtr<Image> image, bool useAlpha)
{
	if (!image)
	{
		WarningString("Null image, can not set data");
		return false;
	}

	// Use a shared ptr for managing the temporary mip images created during this function
	SharedPtr<Image> mipImage;
	unsigned memoryUse = sizeof(Texture2D);
	MaterialQuality quality = MaterialQuality::High;
	auto renderer = GetSubSystem<Renderer>();
	if (renderer)
		quality = renderer->GetTextureQuality();

	if (!image->IsCompressed())
	{
		// Convert unsuitable formats to RGBA
		unsigned components = image->GetComponents();
		if (((components == 1 && !useAlpha) || components == 2))
		{
			mipImage = image->ConvertToRGBA(); image = mipImage;
			if (!image)
				return false;
			components = image->GetComponents();
		}

		unsigned char* levelData = image->GetData().get();
		int levelWidth = image->GetWidth();
		int levelHeight = image->GetHeight();
		unsigned format = 0;

		// Discard unnecessary mip levels
		for (unsigned i = 0; i < _mipsToSkip[(int)quality]; ++i)
		{
			mipImage = image->GetNextLevel(); image = mipImage;
			levelData = image->GetData().get();
			levelWidth = image->GetWidth();
			levelHeight = image->GetHeight();
		}

		switch (components)
		{
		case 1:
			format = useAlpha ? Graphics::GetAlphaFormat() : Graphics::GetLuminanceFormat();
			break;

		case 2:
			format = Graphics::GetLuminanceAlphaFormat();
			break;

		case 3:
			format = Graphics::GetRGBFormat();
			break;

		case 4:
			format = Graphics::GetRGBAFormat();
			break;

		default:
			assert(false);  // Should not reach here
			break;
		}

		// If image was previously compressed, reset number of requested levels to avoid error if level count is too high for new size
		if (IsCompressed() && _requestedLevels > 1)
			_requestedLevels = 0;
		SetSize(levelWidth, levelHeight, format);
		if (!_object.name)
			return false;

		for (unsigned i = 0; i < _levels; ++i)
		{
			SetData(i, 0, 0, levelWidth, levelHeight, levelData);
			memoryUse += levelWidth * levelHeight * components;

			if (i < _levels - 1)
			{
				mipImage = image->GetNextLevel(); image = mipImage;
				levelData = image->GetData().get();
				levelWidth = image->GetWidth();
				levelHeight = image->GetHeight();
			}
		}
	}
	else
	{
		int width = image->GetWidth();
		int height = image->GetHeight();
		unsigned levels = image->GetNumCompressedLevels();
		unsigned format = _graphics.lock()->GetFormat(image->GetCompressedFormat());
		bool needDecompress = false;

		if (!format)
		{
			format = Graphics::GetRGBAFormat();
			needDecompress = true;
		}

		unsigned mipsToSkip = _mipsToSkip[(int)quality];
		if (mipsToSkip >= levels)
			mipsToSkip = levels - 1;
		while (mipsToSkip && (width / (1u << mipsToSkip) < 4 || height / (1u << mipsToSkip) < 4))
			--mipsToSkip;
		width /= (1u << mipsToSkip);
		height /= (1u << mipsToSkip);

		SetNumLevels(max((levels - mipsToSkip), 1U));
		SetSize(width, height, format);

		for (unsigned i = 0; i < _levels && i < levels - mipsToSkip; ++i)
		{
			CompressedLevel level = image->GetCompressedLevel(i + mipsToSkip);
			if (!needDecompress)
			{
				SetData(i, 0, 0, level._width, level._height, level._data);
				memoryUse += level._rows * level._rowSize;
			}
			else
			{
				auto* rgbaData = new unsigned char[level._width * level._height * 4];
				level.Decompress(rgbaData);
				SetData(i, 0, 0, level._width, level._height, rgbaData);
				memoryUse += level._width * level._height * 4;
				delete[] rgbaData;
			}
		}
	}

	SetMemoryUse(memoryUse);
	return true;
}


bool Texture2D::create()
{
	Release();

	if (!_graphics.lock() || !_width || !_height)
		return false;

	if (_graphics.lock()->IsDeviceLost())
	{
		LogString("Texture creation while device is lost");
		return true;
	}


	unsigned format = _format;
	unsigned externalFormat = _format;
	unsigned dataType = GetDataType(_format);
	// Create a renderbuffer instead of a texture if depth texture is not properly supported, or if this will be a packed
   // depth stencil texture
	if (format == GL_DEPTH24_STENCIL8)
	{
		if (_renderSurface)
		{
			_renderSurface->CreateRenderBuffer(_width, _height, format, _multiSample);
			return true;
		}
		else
			return false;
	}
	else
	{
		if (_multiSample > 1)
		{
			if (_autoResolve)
			{
				// Multisample with autoresolve: create a renderbuffer for rendering, but also a texture
				_renderSurface->CreateRenderBuffer(_width, _height, format, _multiSample);
			}
			else
			{
				_target = GL_TEXTURE_2D_MULTISAMPLE;
				if (_renderSurface)
					_renderSurface->_target = GL_TEXTURE_2D_MULTISAMPLE;
			}
		}
	}
	glGenTextures(1, &_object.name);

	// Ensure that our texture is bound to OpenGL texture unit 0
	_graphics.lock()->SetTextureForUpdate(SharedFromThis(Texture2D));

	// If not compressed, create the initial level 0 texture with null data
	bool success = true;

	if (!IsCompressed())
	{
		glGetError();
		if (_multiSample > 1 && !_autoResolve)
		{
			glTexImage2DMultisample(_target, _multiSample, format, _width, _height, GL_TRUE);
		}
		else
		{
			glTexImage2D(_target, 0, format, _width, _height, 0, externalFormat, dataType, nullptr);
		}
		if (glGetError())
		{
			ErrorString("Failed to create texture");
			success = false;
		}
	}

	// Set mipmapping
	if (_usage == TextureUsage::DepthStencil || _usage == TextureUsage::Dynamic)
		_requestedLevels = 1;
	else if (_usage == TextureUsage::RenderTarget)
	{
		if (_requestedLevels != 1)
		{
			// Generate levels for the first time now
			RegenerateLevels();
			// Determine max. levels automatically
			_requestedLevels = 0;
		}
	}

	_levels = CheckMaxLevels(_width, _height, _requestedLevels);

	glTexParameteri(_target, GL_TEXTURE_BASE_LEVEL, 0);
	glTexParameteri(_target, GL_TEXTURE_MAX_LEVEL, _levels - 1);

	// Set initial parameters, then unbind the texture
	UpdateParameters();
	_graphics.lock()->SetTexture(0, nullptr);

	return success;
}

}
#endif