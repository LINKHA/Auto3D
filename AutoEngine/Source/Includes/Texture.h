#pragma once
#include "ResourceWithMetaData.h"
#include "GPUObject.h"
#include "GraphicsDef.h"

namespace Auto3D {

static const int MAX_TEXTURE_QUALITY_LEVELS = 3;



class Texture : public ResourceWithMetaData, public GPUObject
{
	REGISTER_OBJECT_ABSTRACT_CLASS(Texture, ResourceWithMetaData)
public:
	explicit Texture(SharedPtr<Ambient> ambient);
	/// Define texture type and dimensions and set initial data. %ImageLevel structures only need the data pointer and row byte size filled. Return true on success.
	bool Define(TextureType type, ResourceUsage usage, const Vector2I& size, ImageFormat format, size_t numLevels, const ImageLevel* initialData = 0);
	/// Define sampling parameters. Return true on success.
	bool DefineSampler(TextureFilterMode filter = FILTER_TRILINEAR, TextureAddressMode u = ADDRESS_WRAP, TextureAddressMode v = ADDRESS_WRAP, TextureAddressMode w = ADDRESS_WRAP, unsigned maxAnisotropy = 16, float minLod = -M_MAX_FLOAT, float maxLod = M_MAX_FLOAT, const Color& borderColor = Color::BLACK);
	/// Set data for a mipmap level. Not supported for immutable textures. Return true on success.
	bool SetData(size_t face, size_t level, RectInt rect, const ImageLevel& data);

	/// Return whether is a color rendertarget texture.
	bool IsRenderTarget() const { return _usage == ResourceUsage::RenderTarget && (_format < ImageFormat::D16 || _format > ImageFormat::D24S8); }
	/// Return whether is a depth-stencil texture.
	bool IsDepthStencil() const { return _usage == ResourceUsage::RenderTarget && _format >= ImageFormat::D16 && _format <= ImageFormat::D24S8; }
	/// Return texture type.
	TextureType TexType() const { return _type; }
	/// Return dimensions.
	const Vector2I& Size() const { return _size; }
	/// Return width.
	int Width() const { return _size._x; }
	/// Return height.
	int Height() const { return _size._y; }
	/// Return image format.
	ImageFormat Format() const { return _format; }
	/// Return whether uses a compressed format.
	bool IsCompressed() const { return _format >= ImageFormat::DXT1; }
	/// Return number of mipmap levels.
	size_t NumLevels() const { return _numLevels; }
	/// Return the OpenGL texture identifier. Used internally and should not be called by portable application code.
	unsigned GLTexture() const { return _texture; }
	/// Return the OpenGL binding target of the texture. Used internally and should not be called by portable application code.
	unsigned GLTarget() const;

	size_t Texture::NumFaces() const;

	/// Texture filtering mode.
	TextureFilterMode filter;
	/// Texture addressing modes for each coordinate axis.
	TextureAddressMode addressModes[3];
	/// Maximum anisotropy.
	unsigned maxAnisotropy;
	/// Minimum LOD.
	float minLod;
	/// Maximum LOD.
	float maxLod;
	/// Border color. Only effective in border addressing mode.
	Color borderColor;
private:
	/// OpenGL texture object identifier.
	unsigned _texture;
	/// Texture type.
	TextureType _type;
	/// Texture usage mode.
	ResourceUsage _usage;
	/// Texture dimensions in pixels.
	Vector2I _size;
	/// Image format.
	ImageFormat _format;
	/// Number of mipmap levels.
	size_t _numLevels;
	/// Images used for loading.
	VECTOR<SharedPtr<Image> > _loadImages;
};

}