#pragma once

#include "Container/AutoPtr.h"
#include "Math/Vector2.h"
#include "Resource/Resource.h"

struct SDL_Surface;

namespace Auto3D
{

/// AImage formats.
namespace EImageFormat
{
	enum Type
	{
		NONE = 0,
		R8,
		RG8,
		RGBA8,
		A8,
#ifndef AUTO_OPENGL_ES
		R16,
		RG16,
		RGBA16,
#endif
		R16F,
		RG16F,
		RGBA16F,
		R32F,
		RG32F,
		RGB32F,
		RGBA32F,
		D16,
#ifndef AUTO_OPENGL_ES
		D32,
#endif
		D24S8,
#ifndef AUTO_OPENGL_ES
		DXT1,
		DXT3,
		DXT5,
#endif
		ETC1,
		PVRTC_RGB_2BPP,
		PVRTC_RGBA_2BPP,
		PVRTC_RGB_4BPP,
		PVRTC_RGBA_4BPP
	};
};


/// Description of image mip level data.
struct AUTO_API FImageLevel
{
    /// Default construct.
    FImageLevel() :
        _data(nullptr),
        _size(TVector2I::ZERO),
        _rowSize(0),
        _rows(0)
    {
    }

    /// Pointer to pixel data.
    unsigned char* _data;
    /// Level size in pixels.
    TVector2I _size;
    /// Row size in bytes.
    size_t _rowSize;
    /// Number of rows.
    size_t _rows;
};

/// %AImage resource.
class AUTO_API AImage : public AResource
{
	DECLARE_CLASS(AImage, AResource)

public:
    /// Construct.
    AImage();
    /// Destruct.
    ~AImage();

    /// Register object factory.
    static void RegisterObject();

    /// Load image from a stream. Return true on success.
    bool BeginLoad(FStream& source) override;
    /// Save the image to a stream. Regardless of original format, the image is saved as png. Compressed image data is not supported. Return true on success.
    bool Save(FStream& dest) override;

    /// Set new image pixel dimensions and format. Setting a compressed format is not supported.
    void SetSize(const TVector2I& newSize, EImageFormat::Type newFormat);
    /// Set new pixel data.
    void SetData(const unsigned char* pixelData);

    /// Return image dimensions in pixels.
    const TVector2I& GetSize() const { return _size; }
    /// Return image width in pixels.
    int GetWidth() const { return _size._x; }
    /// Return image height in pixels.
    int GetHeight() const { return _size._y; }
    /// Return number of components in a pixel. Will return 0 for formats which are not 8 bits per pixel.
    int GetComponents() const { return components[_format]; }
    /// Return byte _size of a pixel. Will return 0 for block compressed formats.
    size_t PixelByteSize() const { return pixelByteSizes[_format]; } 
    /// Return pixel data.
    unsigned char* Data() const { return _data.Get(); }
    /// Return the image format.
    EImageFormat::Type GetFormat() const { return _format; }
#ifndef AUTO_OPENGL_ES 
    /// Return whether is a compressed image.
    bool IsCompressed() const { return _format >= EImageFormat::DXT1; }
#else
	bool IsCompressed() const { return false; }
#endif
    /// Return number of mip levels contained in the image data.
    size_t GetNumLevels() const { return _numLevels; }
    /// Calculate the next mip image with halved width and height. Supports uncompressed 8 bits per pixel images only. Return true on success.
    bool GenerateMipImage(AImage& dest) const;
    /// Return the data for a mip level. Images loaded from eg. PNG or JPG formats will only have one (index 0) level.
    FImageLevel GetLevel(size_t index) const;
	/// Return an SDL surface from the image, or null if failed. Only RGB images are supported. Specify rect to only return partial image. You must free the surface yourself.
	SDL_Surface* GetSDLSurface(const TRectI& rect = TRectI::ZERO) const;
#ifndef AUTO_OPENGL_ES
	/// Decompress a mip level as 8-bit RGBA. Supports compressed images only. Return true on success.
    bool DecompressLevel(unsigned char* dest, size_t levelIndex) const;
#endif
    /// Calculate the data size of an image level.
    static size_t CalculateDataSize(const TVector2I& _size, EImageFormat::Type _format, size_t* numRows = 0, size_t* rowSize = 0);

    /// Pixel components per format.
    static const int components[];
    /// Pixel byte sizes per format.
    static const size_t pixelByteSizes[];

private:
    /// Decode image pixel data using the stb_image library.
    static unsigned char* DecodePixelData(FStream& source, int& width, int& height, unsigned& components);
    /// Free the decoded pixel data.
    static void FreePixelData(unsigned char* pixelData);

    /// AImage dimensions.
    TVector2I _size;
    /// AImage format.
    EImageFormat::Type _format;
    /// Number of mip levels. 1 for uncompressed images.
    size_t _numLevels;
    /// AImage pixel data.
    TSharedArrayPtr<unsigned char> _data;
};

}
