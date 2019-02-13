#include "Texture.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "RenderSurface.h"

namespace Auto3D {
static GLenum WRAP_MODES[] =
{
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER
};
static GLenum GetWrapMode(TextureAddressMode mode)
{
	return WRAP_MODES[(int)mode];
}

void Texture::UpdateParameters()
{
	if (!_object.name || !_graphics.lock())
		return;

	// If texture is multisampled, do not attempt to set parameters as it's illegal, just return
	if (_target == GL_TEXTURE_2D_MULTISAMPLE)
	{
		_parametersDirty = false;
		return;
	}

	// Wrapping
	glTexParameteri(_target, GL_TEXTURE_WRAP_S, GetWrapMode(_addressModes[(int)TextureCoordinate::U]));
	glTexParameteri(_target, GL_TEXTURE_WRAP_T, GetWrapMode(_addressModes[(int)TextureCoordinate::V]));
	glTexParameteri(_target, GL_TEXTURE_WRAP_R, GetWrapMode(_addressModes[(int)TextureCoordinate::W]));

	TextureFilterMode filterMode = _filterMode;
	if (filterMode == TextureFilterMode::Default)
		filterMode = _graphics.lock()->GetDefaultTextureFilterMode();

	// Filtering
	switch (filterMode)
	{
	case TextureFilterMode::Nearest:
		if (_levels < 2)
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

	case TextureFilterMode::Bilinear:
		if (_levels < 2)
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case TextureFilterMode::Anisotropic:
	case TextureFilterMode::Trilinear:
		if (_levels < 2)
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		else
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;

	case TextureFilterMode::NearestAnisotropic:
		if (_levels < 2)
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		else
			glTexParameteri(_target, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(_target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;

	default:
		break;

		// Shadow compare
		if (_shadowCompare)
		{
			glTexParameteri(_target, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
			glTexParameteri(_target, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		}
		else
			glTexParameteri(_target, GL_TEXTURE_COMPARE_MODE, GL_NONE);

		glTexParameterfv(_target, GL_TEXTURE_BORDER_COLOR, _borderColor.Data());

		_parametersDirty = false;
	}
}
void Texture::RegenerateLevels()
{
	if (!_object.name)
		return;

	glGenerateMipmap(_target);

	_levelsDirty = false;
}

bool Texture::IsCompressed() const
{
	return _format == COMPRESSED_RGB_PVRTC_4BPPV1_IMG || _format == COMPRESSED_RGBA_PVRTC_4BPPV1_IMG ||
		_format == COMPRESSED_RGB_PVRTC_2BPPV1_IMG || _format == COMPRESSED_RGBA_PVRTC_2BPPV1_IMG;
}

void Texture::SetSRGB(bool enable)
{
	if (_graphics.lock())
		enable &= _graphics.lock()->GetSRGBSupport();

	if (enable != _sRGB)
	{
		_sRGB = enable;
		// If texture had already been created, must recreate it to set the sRGB texture format
		if (_object.name)
			create();

		// If texture in use in the framebuffer, mark it dirty
		if (_graphics.lock() && _graphics.lock()->GetRenderTarget(0) && _graphics.lock()->GetRenderTarget(0)->GetParentTexture() == SharedFromThis(Texture))
			_graphics.lock()->MarkFBODirty();
	}
}

unsigned Texture::GetDataType(unsigned format)
{
	if (format == GL_DEPTH24_STENCIL8)
		return GL_UNSIGNED_INT_24_8;
	else if (format == GL_RG16 || format == GL_RGBA16)
		return GL_UNSIGNED_SHORT;
	else if (format == GL_RGB32F || format == GL_RG32F || format == GL_R32F)
		return GL_FLOAT;
	else if (format == GL_RGBA16F || format == GL_RG16F || format == GL_R16F)
		return GL_HALF_FLOAT;
	else
		return GL_UNSIGNED_BYTE;
}

bool Texture::GetParametersDirty() const
{
	return _parametersDirty;
}

unsigned Texture::GetRowDataSize(int width) const
{
	switch (_format)
	{
	case GL_ALPHA:
	case GL_LUMINANCE:
		return (unsigned)width;

	case GL_LUMINANCE_ALPHA:
		return (unsigned)(width * 2);

	case GL_RGB:
		return (unsigned)(width * 3);
	case GL_RGBA:
	case GL_RG16:
	case GL_RG16F:
	case GL_R32F:
		return (unsigned)(width * 4);

	case GL_R8:
		return (unsigned)width;

	case GL_RG8:
	case GL_R16F:
		return (unsigned)(width * 2);

	case GL_RGBA16:
	case GL_RGBA16F:
		return (unsigned)(width * 8);

	case GL_RGBA32F:
		return (unsigned)(width * 16);
	case COMPRESSED_RGB_PVRTC_4BPPV1_IMG:
	case COMPRESSED_RGBA_PVRTC_4BPPV1_IMG:
		return (unsigned)(width * 4 + 7) >> 3u;

	case COMPRESSED_RGB_PVRTC_2BPPV1_IMG:
	case COMPRESSED_RGBA_PVRTC_2BPPV1_IMG:
		return (unsigned)(width * 2 + 7) >> 3u;

	default:
		return 0;
	}
}

}

#endif