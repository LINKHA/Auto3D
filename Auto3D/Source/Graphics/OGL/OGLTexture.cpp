#include "Debug/Log.h"
#include "Debug/Profiler.h"
#include "Resource/ResourceCache.h"
#include "Scene/GeometryNode.h"
#include "Core/Modules/ModuleManager.h"

#include "OGLGraphics.h"
#include "OGLTexture.h"

#include <glad.h>

#include "Debug/DebugNew.h"

namespace Auto3D
{

static const unsigned glTargets[] = 
{
#ifndef AUTO_OPENGL_ES
    GL_TEXTURE_1D,
#endif
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
#ifndef AUTO_OPENGL_ES
    GL_R16,
    GL_RG16,
    GL_RGBA16,
#endif
    GL_R16F,
    GL_RG16F,
    GL_RGBA16F,
    GL_R32F,
    GL_RG32F,
    GL_RGB32F,
    GL_RGBA32F,
    GL_DEPTH_COMPONENT16,
#ifndef AUTO_OPENGL_ES
    GL_DEPTH_COMPONENT32,
#endif
    GL_DEPTH24_STENCIL8,
#ifndef AUTO_OPENGL_ES
    GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
#endif 
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
#ifndef AUTO_OPENGL_ES
    GL_RED,
    GL_RG,
    GL_RGBA,
#endif
    GL_RED,
    GL_RG,
    GL_RGBA,
    GL_RED,
    GL_RG,
    GL_RGB,
    GL_RGBA,
    GL_DEPTH_COMPONENT,
#ifndef AUTO_OPENGL_ES
    GL_DEPTH_COMPONENT,
#endif
    GL_DEPTH_STENCIL,
#ifndef AUTO_OPENGL_ES
    GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
    GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
#endif
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
#ifndef AUTO_OPENGL_ES
    GL_CLAMP_TO_BORDER,
    GL_MIRROR_CLAMP_EXT
#endif
};

ATexture::ATexture() :
    _texture(0),
    _type(ETextureType::TEX_2D),
    _usage(EResourceUsage::DEFAULT),
    _size(TVector2I::ZERO),
    _format(EImageFormat::NONE)
{
}

ATexture::~ATexture()
{
    Release();
}

void ATexture::Release()
{
    if (_graphics)
    {
        for (size_t i = 0; i < MAX_TEXTURE_UNITS; ++i)
        {
            if (_graphics->GetTexture(i) == this)
                _graphics->SetTexture(i, 0);
        }

        if (_usage == EResourceUsage::RENDERTARGET)
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

void ATexture::Recreate()
{
    // If has a name, attempt to reload through the resource cache
    if (FName().Length())
    {
		ResourceCache* cache = GModuleManager::Get().CacheModule();
        if (cache && cache->ReloadResource(this))
            return;
    }

    // If failed to reload, recreate the texture without data and mark data lost
    Define(_type, _usage, _size, _format, _numLevels);
    SetDataLost(true);
}

bool ATexture::Define(ETextureType::Type type, EResourceUsage::Type usage, const TVector2I& size, EImageFormat::Type format, size_t numLevels, const FImageLevel* initialData)
{
	PROFILE(DefineTexture);

	Release();

	if (type != ETextureType::TEX_2D && type != ETextureType::TEX_CUBE)
	{
		ErrorString("Only 2D textures and cube maps supported for now");
		return false;
	}
#ifndef AUTO_OPENGL_ES 
	if (format > EImageFormat::DXT5)
	{
		ErrorString("ETC1 and PVRTC formats are unsupported");
		return false;
	}
#endif
	if (type == ETextureType::TEX_CUBE && size._x != size._y)
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
			_size = TVector2I::ZERO;
			_format = EImageFormat::NONE;
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
			if (_type == ETextureType::TEX_2D)
				glTexImage2D(glTargets[_type], 0, glInternalFormats[_format], _size._x, _size._y, 0, glFormats[_format], glDataTypes[_format], 0);
			else if (_type == ETextureType::TEX_CUBE)
			{
				for (size_t i = 0; i < MAX_CUBE_FACES; ++i)
					glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, glInternalFormats[_format], _size._x, _size._y, 0, glFormats[_format], glDataTypes[_format], 0);
			}
		}

		if (initialData)
		{
			// Hack for allowing immutable texture to set initial data
 			_usage = EResourceUsage::DEFAULT;
			size_t idx = 0;
			for (size_t i = 0; i < GetNumFaces(); ++i)
			{
				for (size_t j = 0; j < _numLevels; ++j)
					SetData(i, j, TRectI(0, 0, Max(_size._x >> j, 1), Max(_size._y >> j, 1)), initialData[idx++]);
			}
			_usage = usage;
		}

		// If we have an error now, the texture was not created correctly
		if (glGetError() != GL_NO_ERROR)
		{
			Release();
			_size = TVector2I::ZERO;
			_format = EImageFormat::NONE;
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
bool ATexture::DefineSampler(ETextureFilterMode::Type filter, ETextureAddressMode::Type u, ETextureAddressMode::Type v, ETextureAddressMode::Type w, unsigned maxAnisotropy, float minLod, float maxLod, const FColor& borderColor)
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
        case ETextureFilterMode::FILTER_POINT:
        case ETextureFilterMode::COMPARE_POINT:
            glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(glTargets[_type], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            break;

        case ETextureFilterMode::FILTER_BILINEAR:
        case ETextureFilterMode::COMPARE_BILINEAR:
            if (_numLevels < 2)
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            else
                glTexParameteri(glTargets[_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glTexParameteri(glTargets[_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            break;

        case ETextureFilterMode::FILTER_ANISOTROPIC:
        case ETextureFilterMode::FILTER_TRILINEAR:
        case ETextureFilterMode::COMPARE_ANISOTROPIC:
        case ETextureFilterMode::COMPARE_TRILINEAR:
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

#ifndef AUTO_OPENGL_ES
		glTexParameterf(glTargets[_type], GL_TEXTURE_MAX_ANISOTROPY_EXT, _filter == ETextureFilterMode::FILTER_ANISOTROPIC ?
			_maxAnisotropy : 1.0f);
		glTexParameterfv(glTargets[_type], GL_TEXTURE_BORDER_COLOR, _borderColor.Data());
#endif 

        glTexParameterf(glTargets[_type], GL_TEXTURE_MIN_LOD, _minLod);
        glTexParameterf(glTargets[_type], GL_TEXTURE_MAX_LOD, _maxLod);

      
        
        if (_filter >= ETextureFilterMode::COMPARE_POINT)
        {
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
        }
        else
            glTexParameteri(glTargets[_type], GL_TEXTURE_COMPARE_MODE, GL_NONE);
    }

    return true;
}

bool ATexture::SetData(size_t face, size_t level, TRectI rect, const FImageLevel& data)
{
    PROFILE(UpdateTextureLevel);

    if (_texture)
    {
        if (_usage == EResourceUsage::IMMUTABLE)
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

		TRectI levelRect(0, 0, Max(_size._x >> level, 1), Max(_size._y >> level, 1));
        if (levelRect.IsInside(rect) != INSIDE)
        {
            ErrorStringF("Texture update region %s is outside level %s", rect.ToString().CString(), levelRect.ToString().CString());
            return false;
        }

        // Bind for updating
        _graphics->SetTexture(0, this);

        bool wholeLevel = rect == levelRect;
        unsigned target = (_type == ETextureType::TEX_CUBE) ? GL_TEXTURE_CUBE_MAP_POSITIVE_X + face: glTargets[_type];
        
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
                    0, (unsigned)AImage::CalculateDataSize(TVector2I(rect.Width(), rect.Height()), _format), data._data);
            }
            else
            {
                glCompressedTexSubImage2D(target, (unsigned)level, rect.Left(), rect.Top(), rect.Width(), rect.Height(),
                    glFormats[_format], (unsigned)AImage::CalculateDataSize(TVector2I(rect.Width(), rect.Height()), _format),
                    data._data);
            }
        }
    }

    return true;
}

unsigned ATexture::GetGLTarget() const
{
    return glTargets[_type];
}
Geometry* ATexture::GetGeometry() const
{ 
	return _geometry; 
}

}
