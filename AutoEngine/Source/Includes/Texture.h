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

	/// Return whether is a color rendertarget texture.
	bool IsRenderTarget() const { return _usage == ResourceUsage::RenderTarget && (_format < ImageFormat::D16 || _format > ImageFormat::D24S8); }
	/// Return whether is a depth-stencil texture.
	bool IsDepthStencil() const { return _usage == ResourceUsage::RenderTarget && _format >= ImageFormat::D16 && _format <= ImageFormat::D24S8; }
	/// Return texture type.
	TextureType TexType() const { return _type; }
	/// Return dimensions.
	const Vector2& Size() const { return _size; }
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
private:
	/// OpenGL texture object identifier.
	unsigned _texture;
	/// Texture type.
	TextureType _type;
	/// Texture usage mode.
	ResourceUsage _usage;
	/// Texture dimensions in pixels.
	Vector2 _size;
	/// Image format.
	ImageFormat _format;
	/// Number of mipmap levels.
	size_t _numLevels;
	/// Images used for loading.
	VECTOR<SharedPtr<Image> > _loadImages;
};

}