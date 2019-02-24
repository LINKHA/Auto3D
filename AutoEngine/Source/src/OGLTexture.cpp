#include "Texture.h"
#if AUTO_OPENGL
#include "AutoOGL.h"


namespace Auto3D {

static const unsigned glTargets[] =
{
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP
};

static const unsigned glInternalFormats[] =
{
	0,
	GL_R8,
	GL_RG8,
	GL_RGBA8,
	GL_ALPHA,
	GL_R16,
	GL_RG16,
	GL_RGBA16,
	GL_R16F,
	GL_RG16F,
	GL_RGBA16F,
	GL_R32F,
	GL_RG32F,
	GL_RGB32F,
	GL_RGBA32F,
	GL_DEPTH_COMPONENT16,
	GL_DEPTH_COMPONENT32,
	GL_DEPTH24_STENCIL8,
	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
	0,
	0,
	0,
	0,
	0
};

static const unsigned glFormats[] =
{
	0,
	GL_RED,
	GL_RG,
	GL_RGBA,
	GL_ALPHA,
	GL_RED,
	GL_RG,
	GL_RGBA,
	GL_RED,
	GL_RG,
	GL_RGBA,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA,
	GL_DEPTH_COMPONENT,
	GL_DEPTH_COMPONENT,
	GL_DEPTH_STENCIL,
	GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
	GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
	GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
	0,
	0,
	0,
	0,
	0
};

static const unsigned glDataTypes[] =
{
	0,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_SHORT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_FLOAT,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT,
	GL_UNSIGNED_INT_24_8,
	0,
	0,
	0,
	0,
	0,
	0,
	0,
	0
};

static const unsigned glWrapModes[] =
{
	0,
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
};
unsigned Texture::GLTarget() const
{
	return glTargets[(int)_type];
}

bool Texture::Define(TextureType type_, ResourceUsage usage_, const Vector2I& size_, ImageFormat format_, size_t numLevels_, const ImageLevel* initialData)
{
	Release();

	if (type_ != TextureType::TwoD && type_ != TextureType::Cube)
	{
		ErrorString("Only 2D textures and cube maps supported for now");
		return false;
	}
	if (format_ > ImageFormat::DXT5)
	{
		ErrorString("ETC1 and PVRTC formats are unsupported");
		return false;
	}
	if (type_ == TextureType::Cube && size_._x != size_._y)
	{
		ErrorString("Cube map must have square dimensions");
		return false;
	}

	if (numLevels_ < 1)
		numLevels_ = 1;

	_type = type_;
	_usage = usage_;

	if (_graphics.lock() && _graphics.lock()->IsInitialized())
	{
		glGenTextures(1, &_texture);
		if (!_texture)
		{
			_size = Vector2I::ZERO;
			_format = ImageFormat::NONE;
			_numLevels = 0;

			ErrorString("Failed to create texture");
			return false;
		}

		// Ensure the texture is bound for creation
		_graphics.lock()->SetTexture(0, SharedFromThis(Texture));

		_size = size_;
		_format = format_;
		_numLevels = numLevels_;

		// If not compressed and no initial data, create the initial level 0 texture with null data
		// Clear previous error first to be able to check whether the data was successfully set
		glGetError();
		if (!IsCompressed() && !initialData)
		{
			if (_type == TextureType::TwoD)
				glTexImage2D(glTargets[(int)_type], 0, glInternalFormats[(int)_format], _size._x, _size._y, 0, glFormats[(int)_format], glDataTypes[(int)_format], 0);
			else if (_type == TextureType::Cube)
			{
				for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormats[(int)_format], _size._x, _size._y, 0, glFormats[(int)_format], glDataTypes[(int)_format], 0);
			}
		}

		if (initialData)
		{
			// Hack for allowing immutable texture to set initial data
			_usage = ResourceUsage::Default;
			size_t idx = 0;
			for (size_t i = 0; i < NumFaces(); ++i)
			{
				for (size_t j = 0; j < _numLevels; ++j)
					SetData(i, j, RectInt(0, 0, Max(_size._x >> j, 1), Max(_size._y >> j, 1)), initialData[idx++]);
			}
			_usage = usage_;
		}

		// If we have an error now, the texture was not created correctly
		if (glGetError() != GL_NO_ERROR)
		{
			Release();
			_size = Vector2I::ZERO;
			_format = ImageFormat::NONE;
			_numLevels = 0;

			ErrorString("Failed to create texture");
			return false;
		}

		glTexParameteri(glTargets[(int)_type], GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MAX_LEVEL, (unsigned)_numLevels - 1);
		LogString("Created texture width");
	}

	return true;
}

bool Texture::DefineSampler(TextureFilterMode filter_, TextureAddressMode u, TextureAddressMode v, TextureAddressMode w, unsigned maxAnisotropy_, float minLod_, float maxLod_, const Color& borderColor_)
{
	
	filter = filter_;
	addressModes[0] = u;
	addressModes[1] = v;
	addressModes[2] = w;
	maxAnisotropy = maxAnisotropy_;
	minLod = minLod_;
	maxLod = maxLod_;
	borderColor = borderColor_;

	if (_graphics.lock() && _graphics.lock()->IsInitialized())
	{
		if (!_texture)
		{
			ErrorString("On OpenGL texture must be defined before defining sampling parameters");
			return false;
		}

		// Bind for defining sampling parameters
		_graphics.lock()->SetTexture(0, SharedFromThis(Texture));

		switch (filter)
		{
		case FILTER_POINT:
		case COMPARE_POINT:
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;

		case FILTER_BILINEAR:
		case COMPARE_BILINEAR:
			if (_numLevels < 2)
				glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else
				glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		case FILTER_ANISOTROPIC:
		case FILTER_TRILINEAR:
		case COMPARE_ANISOTROPIC:
		case COMPARE_TRILINEAR:
			if (_numLevels < 2)
				glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			else
				glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;

		default:
			break;
		}

		glTexParameteri(glTargets[(int)_type], GL_TEXTURE_WRAP_S, glWrapModes[addressModes[0]]);
		glTexParameteri(glTargets[(int)_type], GL_TEXTURE_WRAP_T, glWrapModes[addressModes[1]]);
		glTexParameteri(glTargets[(int)_type], GL_TEXTURE_WRAP_R, glWrapModes[addressModes[2]]);

		glTexParameterf(glTargets[(int)_type], GL_TEXTURE_MAX_ANISOTROPY, filter == FILTER_ANISOTROPIC ?
			maxAnisotropy : 1.0f);

		glTexParameterf(glTargets[(int)_type], GL_TEXTURE_MIN_LOD, minLod);
		glTexParameterf(glTargets[(int)_type], GL_TEXTURE_MAX_LOD, maxLod);

		glTexParameterfv(glTargets[(int)_type], GL_TEXTURE_BORDER_COLOR, borderColor.Data());

		if (filter >= COMPARE_POINT)
		{
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}
		else
			glTexParameteri(glTargets[(int)_type], GL_TEXTURE_COMPARE_MODE, GL_NONE);
	}

	return true;
}

bool Texture::SetData(size_t face, size_t level, RectInt rect, const ImageLevel& data)
{
	if (_texture)
	{
		if (_usage == ResourceUsage::Immutable)
		{
			ErrorString("Can not update immutable texture");
			return false;
		}
		if (face >= NumFaces())
		{
			ErrorString("Face to update out of bounds");
			return false;
		}
		if (level >= _numLevels)
		{
			ErrorString("Mipmap level to update out of bounds");
			return false;
		}

		RectInt levelRect(0, 0, Max(_size._x >> level, 1), Max(_size._y >> level, 1));
		if (levelRect.IsInside(rect) != INSIDE)
		{
			ErrorString("Texture update region");
			return false;
		}

		// Bind for updating
		_graphics.lock()->SetTexture(0, SharedFromThis(Texture));

		bool wholeLevel = rect == levelRect;
		unsigned target = (_type == TextureType::Cube) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face : glTargets[(int)_type];

		if (!IsCompressed())
		{
			if (wholeLevel)
			{
				glTexImage2D(target, (unsigned)level, glInternalFormats[(int)_format], rect.Width(), rect.Height(), 0,
					glFormats[(int)_format], glDataTypes[(int)_format], data.data);
			}
			else
			{
				glTexSubImage2D(target, (unsigned)level, rect._left, rect._top, rect.Width(), rect.Height(),
					glFormats[(int)_format], glDataTypes[(int)_format], data.data);
			}
		}
		else
		{
			if (wholeLevel)
			{
				glCompressedTexImage2D(target, (unsigned)level, glInternalFormats[(int)_format], rect.Width(), rect.Height(),
					0, (unsigned)Image::CalculateDataSize(Vector2I(rect.Width(), rect.Height()), _format), data.data);
			}
			else
			{
				glCompressedTexSubImage2D(target, (unsigned)level, rect._left, rect._top, rect.Width(), rect.Height(),
					glFormats[(int)_format], (unsigned)Image::CalculateDataSize(Vector2I(rect.Width(), rect.Height()), _format),
					data.data);
			}
		}
	}

	return true;
}

}

#endif