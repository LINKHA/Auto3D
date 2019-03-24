#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "../../Resource/ResourceCache.h"
#include "../../Renderer/GeometryNode.h"

#include "OGLGraphics.h"
#include "OGLTexture.h"

#include <glad.h>

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

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
    GL_MIRROR_CLAMP_EXT
};

Texture::Texture() :
    _texture(0),
    _type(TextureType::TEX_2D),
    _usage(ResourceUsage::DEFAULT),
    _size(Vector2I::ZERO),
    _format(ImageFormat::NONE)
{
}

Texture::~Texture()
{
    Release();
}

void Texture::Release()
{
    if (_graphics)
    {
        for (size_t i = 0; i < MAX_TEXTURE_UNITS; ++i)
        {
            if (_graphics->GetTexture(i) == this)
                _graphics->SetTexture(i, 0);
        }

        if (_usage == ResourceUsage::RENDERTARGET)
        {
            bool clear = false;

            for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
            {
                if (_graphics->RenderTarget(i) == this)
                {
                    clear = true;
                    break;
                }
            }

            if (!clear && _graphics->DepthStencil() == this)
                clear = true;

            if (clear)
                _graphics->ResetRenderTargets();

            // Clear from all FBO's
            _graphics->CleanupFramebuffers(this);
        }
    }

    if (_texture)
    {
        glDeleteTextures(1, &_texture);
        _texture = 0;
    }
}

void Texture::Recreate()
{
    // If has a name, attempt to reload through the resource cache
    if (Name().Length())
    {
        ResourceCache* cache = Subsystem<ResourceCache>();
        if (cache && cache->ReloadResource(this))
            return;
    }

    // If failed to reload, recreate the texture without data and mark data lost
    Define(_type, _usage, _size, _format, _numLevels);
    SetDataLost(true);
}

bool Texture::Define(TextureType type, ResourceUsage usage, const Vector2I& size, ImageFormat format, size_t numLevels, const ImageLevel* initialData)
{
	PROFILE(DefineTexture);

	Release();

	if (type != TextureType::TEX_2D && type != TextureType::TEX_CUBE)
	{
		ErrorString("Only 2D textures and cube maps supported for now");
		return false;
	}
	if (format > ImageFormat::DXT5)
	{
		ErrorString("ETC1 and PVRTC formats are unsupported");
		return false;
	}
	if (type == TextureType::TEX_CUBE && size._x != size._y)
	{
		ErrorString("Cube map must have square dimensions");
		return false;
	}

	if (numLevels < 1)
		numLevels = 1;

	_type = type;
	_usage = usage;

	if (_graphics && _graphics->IsInitialized())
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
		_graphics->SetTexture(0, this);

		_size = size;
		_format = format;
		_numLevels = numLevels;

		// If not compressed and no initial data, create the initial level 0 texture with null data
		// Clear previous error first to be able to check whether the data was successfully set
		glGetError();
		if (!IsCompressed() && !initialData)
		{
			if (_type == TextureType::TEX_2D)
				glTexImage2D(glTargets[_type], 0, glInternalFormats[_format], _size._x, _size._y, 0, glFormats[_format], glDataTypes[_format], 0);
			else if (_type == TextureType::TEX_CUBE)
			{
				for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormats[_format], _size._x, _size._y, 0, glFormats[_format], glDataTypes[_format], 0);
			}
		}

		if (initialData)
		{
			// Hack for allowing immutable texture to set initial data
			_usage = ResourceUsage::DEFAULT;
			size_t idx = 0;
			for (size_t i = 0; i < GetNumFaces(); ++i)
			{
				for (size_t j = 0; j < _numLevels; ++j)
					SetData(i, j, RectI(0, 0, Max(_size._x >> j, 1), Max(_size._y >> j, 1)), initialData[idx++]);
			}
			_usage = usage;
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

		glTexParameteri(glTargets[_type], GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(glTargets[_type], GL_TEXTURE_MAX_LEVEL, (unsigned)_numLevels - 1);
		LogStringF("Created texture width %d height %d format %d numLevels %d", _size._x, _size._y, (int)_format, _numLevels);
	}

	return true;
}
bool Texture::DefineSampler(TextureFilterMode filter, TextureAddressMode u, TextureAddressMode v, TextureAddressMode w, unsigned maxAnisotropy, float minLod, float maxLod, const Color& borderColor)
{
    PROFILE(DefineTextureSampler);

    _filter = filter;
    _addressModes[0] = u;
    _addressModes[1] = v;
    _addressModes[2] = w;
    _maxAnisotropy = maxAnisotropy;
    _minLod = minLod;
    _maxLod = maxLod;
    _borderColor = borderColor;

    if (_graphics && _graphics->IsInitialized())
    {
        if (!_texture)
        {
            ErrorString("On OpenGL texture must be defined before defining sampling parameters");
            return false;
        }

        // Bind for defining sampling parameters
        _graphics->SetTexture(0, this);

        switch (_filter)
        {
        case TextureFilterMode::FILTER_POINT:
        case TextureFilterMode::COMPARE_POINT:
            glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(glTargets[_type], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case TextureFilterMode::FILTER_BILINEAR:
        case TextureFilterMode::COMPARE_BILINEAR:
            if (_numLevels < 2)
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            else
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(glTargets[_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        case TextureFilterMode::FILTER_ANISOTROPIC:
        case TextureFilterMode::FILTER_TRILINEAR:
        case TextureFilterMode::COMPARE_ANISOTROPIC:
        case TextureFilterMode::COMPARE_TRILINEAR:
            if (_numLevels < 2)
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            else
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(glTargets[_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        default:
            break;
        }

        glTexParameteri(glTargets[_type], GL_TEXTURE_WRAP_S, glWrapModes[_addressModes[0]]);
        glTexParameteri(glTargets[_type], GL_TEXTURE_WRAP_T, glWrapModes[_addressModes[1]]);
        glTexParameteri(glTargets[_type], GL_TEXTURE_WRAP_R, glWrapModes[_addressModes[2]]);

        glTexParameterf(glTargets[_type], GL_TEXTURE_MAX_ANISOTROPY_EXT, _filter == TextureFilterMode::FILTER_ANISOTROPIC ?
            _maxAnisotropy : 1.0f);

        glTexParameterf(glTargets[_type], GL_TEXTURE_MIN_LOD, _minLod);
        glTexParameterf(glTargets[_type], GL_TEXTURE_MAX_LOD, _maxLod);

        glTexParameterfv(glTargets[_type], GL_TEXTURE_BORDER_COLOR, _borderColor.Data());
        
        if (_filter >= TextureFilterMode::COMPARE_POINT)
        {
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
        else
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }

    return true;
}

bool Texture::SetData(size_t face, size_t level, RectI rect, const ImageLevel& data)
{
    PROFILE(UpdateTextureLevel);

    if (_texture)
    {
        if (_usage == ResourceUsage::IMMUTABLE)
        {
            ErrorString("Can not update immutable texture");
            return false;
        }
        if (face >= GetNumFaces())
        {
            ErrorString("Face to update out of bounds");
            return false;
        }
        if (level >= _numLevels)
        {
            ErrorString("Mipmap level to update out of bounds");
            return false;
        }

		RectI levelRect(0, 0, Max(_size._x >> level, 1), Max(_size._y >> level, 1));
        if (levelRect.IsInside(rect) != INSIDE)
        {
            ErrorStringF("Texture update region %s is outside level %s", rect.ToString().CString(), levelRect.ToString().CString());
            return false;
        }

        // Bind for updating
        _graphics->SetTexture(0, this);

        bool wholeLevel = rect == levelRect;
        unsigned target = (_type == TextureType::TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face: glTargets[_type];
        
        if (!IsCompressed())
        {
            if (wholeLevel)
            {
                glTexImage2D(target, (unsigned)level, glInternalFormats[_format], rect.Width(), rect.Height(), 0,
                    glFormats[_format], glDataTypes[_format], data._data);
            }
            else
            {
                glTexSubImage2D(target, (unsigned)level, rect.Left(), rect.Top(), rect.Width(), rect.Height(), 
                    glFormats[_format], glDataTypes[_format], data._data);
            }
        }
        else
        {
            if (wholeLevel)
            {
                glCompressedTexImage2D(target, (unsigned)level, glInternalFormats[_format], rect.Width(), rect.Height(),
                    0, (unsigned)Image::CalculateDataSize(Vector2I(rect.Width(), rect.Height()), _format), data._data);
            }
            else
            {
                glCompressedTexSubImage2D(target, (unsigned)level, rect.Left(), rect.Top(), rect.Width(), rect.Height(),
                    glFormats[_format], (unsigned)Image::CalculateDataSize(Vector2I(rect.Width(), rect.Height()), _format),
                    data._data);
            }
        }
    }

    return true;
}

unsigned Texture::GetGLTarget() const
{
    return glTargets[_type];
}
Geometry* Texture::GetGeometry() const
{ 
	return _geometry; 
}

}
