#pragma once

#include "Math/Color.h"
#include "Math/Rect.h"
#include "Engine/Components/Image.h"
#include "Graphics/GPUObject.h"
#include "Graphics/GraphicsDefs.h"

namespace Auto3D
{

class AImage;
class FGeometry;

/// %ATexture on the GPU.
class AUTO_API ATexture : public AResource, public FGPUObject
{
	REGISTER_OBJECT(ATexture, AResource)

public:
    /// Construct.
    ATexture();
    /// Destruct.
    ~ATexture();

    /// Register object factory.
    static void RegisterObject();

    /// Load the texture image data from a stream. Return true on success.
    bool BeginLoad(FStream& source) override;
    /// Finish texture loading by uploading to the GPU. Return true on success.
    bool EndLoad() override;
    /// Release the texture and sampler objects.
    void Release() override;
    /// Recreate the GPU resource after data loss.
    void Recreate() override;

    /// Define texture type and dimensions and set initial data. %FImageLevel structures only need the data pointer and row byte _size filled. Return true on success.
    bool Define(ETextureType::Type type, EResourceUsage::Type usage, const TVector2I& _size, EImageFormat::Type _format, size_t _numLevels, const FImageLevel* initialData = 0);
    /// Define sampling parameters. Return true on success.
    bool DefineSampler(ETextureFilterMode::Type filter = ETextureFilterMode::FILTER_TRILINEAR, ETextureAddressMode::Type u = ETextureAddressMode::WRAP, ETextureAddressMode::Type v = ETextureAddressMode::WRAP, ETextureAddressMode::Type w = ETextureAddressMode::WRAP, unsigned maxAnisotropy = 16, float minLod = -M_MAX_FLOAT, float maxLod = M_MAX_FLOAT, const FColor& borderColor = FColor::BLACK);
    /// Set data for a mipmap level. Not supported for immutable textures. Return true on success.
    bool SetData(size_t face, size_t level, TRectI rect, const FImageLevel& data);

    /// Return texture type.
    ETextureType::Type GetTexType() const { return _type; }
    /// Return dimensions.
    const TVector2I& GetSize() const { return _size; }
    /// Return width.
    int GetWidth() const { return _size._x; }
    /// Return height.
    int GetHeight() const { return _size._y; }
    /// Return image format.
    EImageFormat::Type GetFormat() const { return _format; }
#ifndef AUTO_OPENGL_ES 
    /// Return whether uses a compressed format.
    bool IsCompressed() const { return _format >= EImageFormat::DXT1; }
#else
	bool IsCompressed() const { return false; }
#endif
    /// Return number of mipmap levels.
    size_t GetNumLevels() const { return _numLevels; }
    /// Return number of faces or Z-slices.
    size_t GetNumFaces() const;
    /// Return resource usage type.
    EResourceUsage::Type GetUsage() const { return _usage; }
    /// Return whether is dynamic.
    bool IsDynamic() const { return _usage == EResourceUsage::DYNAMIC; }
    /// Return whether is immutable.
    bool IsImmutable() const { return _usage == EResourceUsage::IMMUTABLE; }
    /// Return whether is a color rendertarget texture.
    bool IsRenderTarget() const { return _usage == EResourceUsage::RENDERTARGET && (_format < EImageFormat::D16 || _format > EImageFormat::D24S8); }
    /// Return whether is a depth-stencil texture.
    bool IsDepthStencil() const { return _usage == EResourceUsage::RENDERTARGET && _format >= EImageFormat::D16 && _format <= EImageFormat::D24S8; }
	

    /// Return the OpenGL texture identifier. Used internally and should not be called by portable application code.
    unsigned GetGLTexture() const { return _texture; }
    /// Return the OpenGL binding target of the texture. Used internally and should not be called by portable application code.
    unsigned GetGLTarget() const;

	FGeometry* GetGeometry() const;

    /// ATexture filtering mode.
    ETextureFilterMode::Type _filter;
    /// ATexture addressing modes for each coordinate axis.
    ETextureAddressMode::Type _addressModes[3];
    /// Maximum anisotropy.
    unsigned _maxAnisotropy;
    /// Minimum LOD.
    float _minLod;
    /// Maximum LOD.
    float _maxLod;
    /// Border color. Only effective in border addressing mode.
    FColor _borderColor;
	/// OpenGL texture object identifier.
	unsigned _texture;
private:

    /// ATexture type.
    ETextureType::Type _type;
    /// ATexture usage mode.
    EResourceUsage::Type _usage;
    /// ATexture dimensions in pixels.
    TVector2I _size;
    /// AImage format.
    EImageFormat::Type _format;
    /// Number of mipmap levels.
    size_t _numLevels;
    /// Images used for loading.
    TVector<TAutoPtr<AImage> > _loadImages;
	/// Draw call source datas.
	TSharedPtr<FGeometry> _geometry;
};

}
