#include "Texture2D.h"
#include "AutoOGL.h"

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

	_graphics.lock()->SetTextureForUpdate(SharedFromThis());

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
bool Texture2D::SetData(Image* image, bool useAlpha)
{

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
	_graphics.lock()->SetTextureForUpdate(SharedFromThis());

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