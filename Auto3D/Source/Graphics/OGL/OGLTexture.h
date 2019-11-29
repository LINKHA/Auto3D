#pragma once

#include "Math/Color.h"
#include "Math/Rect.h"
#include "Engine/Components/Image.h"
#include "Graphics/GPUObject.h"
#include "Graphics/GraphicsDefs.h"

namespace Auto3D
{

class Image;
class Geometry;

/// %Texture on the GPU.
class AUTO_API Texture : public AResource, public GPUObject
{
	REGISTER_OBJECT_CLASS(Texture, AResource)

public:
    /// Construct.
    Texture();
    /// Destruct.
    ~Texture();

    /// Register object factory.
    static void RegisterObject();

    /// Load the texture image data from a stream. Return true on success.
    bool BeginLoad(Stream& source) override;
    /// Finish texture loading by uploading to the GPU. Return true on success.
    bool EndLoad() override;
    /// Release the texture and sampler objects.
    void Release() override;
    /// Recreate the GPU resource after data loss.
    void Recreate() override;

    /// Define texture type and dimensions and set initial data. %ImageLevel structures only need the data pointer and row byte _size filled. Return true on success.
    bool Define(TextureType::Type type, ResourceUsage::Type usage, const Vector2I& _size, ImageFormat::Type _format, size_t _numLevels, const ImageLevel* initialData = 0);
    /// Define sampling parameters. Return true on success.
    bool DefineSampler(TextureFilterMode::Type filter = TextureFilterMode::FILTER_TRILINEAR, TextureAddressMode::Type u = TextureAddressMode::WRAP, TextureAddressMode::Type v = TextureAddressMode::WRAP, TextureAddressMode::Type w = TextureAddressMode::WRAP, unsigned maxAnisotropy = 16, float minLod = -M_MAX_FLOAT, float maxLod = M_MAX_FLOAT, const Color& borderColor = Color::BLACK);
    /// Set data for a mipmap level. Not supported for immutable textures. Return true on success.
    bool SetData(size_t face, size_t level, RectI rect, const ImageLevel& data);

    /// Return texture type.
    TextureType::Type GetTexType() const { return _type; }
    /// Return dimensions.
    const Vector2I& GetSize() const { return _size; }
    /// Return width.
    int GetWidth() const { return _size._x; }
    /// Return height.
    int GetHeight() const { return _size._y; }
    /// Return image format.
    ImageFormat::Type GetFormat() const { return _format; }
#ifndef AUTO_OPENGL_ES 
    /// Return whether uses a compressed format.
    bool IsCompressed() const { return _format >= ImageFormat::DXT1; }
#else
	bool IsCompressed() const { return false; }
#endif
    /// Return number of mipmap levels.
    size_t GetNumLevels() const { return _numLevels; }
    /// Return number of faces or Z-slices.
    size_t GetNumFaces() const;
    /// Return resource usage type.
    ResourceUsage::Type GetUsage() const { return _usage; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return _usage == ResourceUsage::DYNAMIC; }
    /// Return whether is immutable.
    bool IsImmutable() const { return _usage == ResourceUsage::IMMUTABLE; }
    /// Return whether is a color rendertarget texture.
    bool IsRenderTarget() const { return _usage == ResourceUsage::RENDERTARGET && (_format < ImageFormat::D16 || _format > ImageFormat::D24S8); }
    /// Return whether is a depth-stencil texture.
    bool IsDepthStencil() const { return _usage == ResourceUsage::RENDERTARGET && _format >= ImageFormat::D16 && _format <= ImageFormat::D24S8; }
	

    /// Return the OpenGL texture identifier. Used internally and should not be called by portable application code.
    unsigned GetGLTexture() const { return _texture; }
    /// Return the OpenGL binding target of the texture. Used internally and should not be called by portable application code.
    unsigned GetGLTarget() const;

	Geometry* GetGeometry() const;

    /// Texture filtering mode.
    TextureFilterMode::Type _filter;
    /// Texture addressing modes for each coordinate axis.
    TextureAddressMode::Type _addressModes[3];
    /// Maximum anisotropy.
    unsigned _maxAnisotropy;
    /// Minimum LOD.
    float _minLod;
    /// Maximum LOD.
    float _maxLod;
    /// Border color. Only effective in border addressing mode.
    Color _borderColor;
	/// OpenGL texture object identifier.
	unsigned _texture;
private:

    /// Texture type.
    TextureType::Type _type;
    /// Texture usage mode.
    ResourceUsage::Type _usage;
    /// Texture dimensions in pixels.
    Vector2I _size;
    /// Image format.
    ImageFormat::Type _format;
    /// Number of mipmap levels.
    size_t _numLevels;
    /// Images used for loading.
    TVector<TAutoPtr<Image> > _loadImages;
	/// Draw call source datas.
	TSharedPtr<Geometry> _geometry;
};

}
