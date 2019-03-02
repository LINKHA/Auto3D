#pragma once

#include "../../Math/Color.h"
#include "../../Math/Rect.h"
#include "../../Resource/Image.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Auto3D
{

class Image;
class Geometry;

/// %Texture on the GPU.
class AUTO_API Texture : public Resource, public GPUObject
{
	REGISTER_OBJECT_CLASS(Texture, Resource)

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
    bool Define(TextureType type, ResourceUsage usage, const Vector2I& size, ImageFormat format, size_t numLevels, const ImageLevel* initialData = 0);
    /// Define sampling parameters. Return true on success.
    bool DefineSampler(TextureFilterMode filter = TextureFilterMode::FILTER_TRILINEAR, TextureAddressMode u = TextureAddressMode::WRAP, TextureAddressMode v = TextureAddressMode::WRAP, TextureAddressMode w = TextureAddressMode::WRAP, unsigned maxAnisotropy = 16, float minLod = -M_MAX_FLOAT, float maxLod = M_MAX_FLOAT, const Color& borderColor = Color::BLACK);
    /// Set data for a mipmap level. Not supported for immutable textures. Return true on success.
    bool SetData(size_t face, size_t level, RectI rect, const ImageLevel& data);

    /// Return texture type.
    TextureType GetTexType() const { return _type; }
    /// Return dimensions.
    const Vector2I& GetSize() const { return _size; }
    /// Return width.
    int GetWidth() const { return _size._x; }
    /// Return height.
    int GetHeight() const { return _size._y; }
    /// Return image format.
    ImageFormat GetFormat() const { return _format; }
    /// Return whether uses a compressed format.
    bool IsCompressed() const { return _format >= ImageFormat::DXT1; }
    /// Return number of mipmap levels.
    size_t GetNumLevels() const { return _numLevels; }
    /// Return number of faces or Z-slices.
    size_t GetNumFaces() const;
    /// Return resource usage type.
    ResourceUsage GetUsage() const { return _usage; }
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

    /// Texture filtering mode.
    TextureFilterMode _filter;
    /// Texture addressing modes for each coordinate axis.
    TextureAddressMode _addressModes[3];
    /// Maximum anisotropy.
    unsigned _maxAnisotropy;
    /// Minimum LOD.
    float _minLod;
    /// Maximum LOD.
    float _maxLod;
    /// Border color. Only effective in border addressing mode.
    Color _borderColor;

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
    Vector<AutoPtr<Image> > _loadImages;
};

}
