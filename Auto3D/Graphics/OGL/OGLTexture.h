#pragma once

#include "../../Math/Color.h"
#include "../../Math/IntRect.h"
#include "../../Resource/Image.h"
#include "../GPUObject.h"
#include "../GraphicsDefs.h"

namespace Auto3D
{

class Image;

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

    /// Define texture type and dimensions and set initial data. %ImageLevel structures only need the data pointer and row byte size filled. Return true on success.
    bool Define(TextureType type, ResourceUsage usage, const IntVector2& size, ImageFormat format, size_t numLevels, const ImageLevel* initialData = 0);
    /// Define sampling parameters. Return true on success.
    bool DefineSampler(TextureFilterMode filter = FILTER_TRILINEAR, TextureAddressMode u = ADDRESS_WRAP, TextureAddressMode v = ADDRESS_WRAP, TextureAddressMode w = ADDRESS_WRAP, unsigned maxAnisotropy = 16, float minLod = -M_MAX_FLOAT, float maxLod = M_MAX_FLOAT, const Color& borderColor = Color::BLACK);
    /// Set data for a mipmap level. Not supported for immutable textures. Return true on success.
    bool SetData(size_t face, size_t level, IntRect rect, const ImageLevel& data);

    /// Return texture type.
    TextureType TexType() const { return type; }
    /// Return dimensions.
    const IntVector2& Size() const { return size; }
    /// Return width.
    int Width() const { return size.x; }
    /// Return height.
    int Height() const { return size.y; }
    /// Return image format.
    ImageFormat Format() const { return format; }
    /// Return whether uses a compressed format.
    bool IsCompressed() const { return format >= FMT_DXT1; }
    /// Return number of mipmap levels.
    size_t NumLevels() const { return numLevels; }
    /// Return number of faces or Z-slices.
    size_t NumFaces() const;
    /// Return resource usage type.
    ResourceUsage Usage() const { return usage; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return usage == USAGE_DYNAMIC; }
    /// Return whether is immutable.
    bool IsImmutable() const { return usage == USAGE_IMMUTABLE; }
    /// Return whether is a color rendertarget texture.
    bool IsRenderTarget() const { return usage == USAGE_RENDERTARGET && (format < FMT_D16 || format > FMT_D24S8); }
    /// Return whether is a depth-stencil texture.
    bool IsDepthStencil() const { return usage == USAGE_RENDERTARGET && format >= FMT_D16 && format <= FMT_D24S8; }

    /// Return the OpenGL texture identifier. Used internally and should not be called by portable application code.
    unsigned GLTexture() const { return texture; }
    /// Return the OpenGL binding target of the texture. Used internally and should not be called by portable application code.
    unsigned GLTarget() const;

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
    unsigned texture;
    /// Texture type.
    TextureType type;
    /// Texture usage mode.
    ResourceUsage usage;
    /// Texture dimensions in pixels.
    IntVector2 size;
    /// Image format.
    ImageFormat format;
    /// Number of mipmap levels.
    size_t numLevels;
    /// Images used for loading.
    Vector<AutoPtr<Image> > loadImages;
};

}
