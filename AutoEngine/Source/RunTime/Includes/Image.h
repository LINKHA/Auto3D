#pragma once
#include "Auto.h"
#include "Resource.h"

namespace Auto3D {

struct SDL_Surface;


static const int COLOR_LUT_SIZE = 16;

/// Supported compressed image formats.
enum CompressedFormat
{
	NONE = 0,
	RGBA,
	DXT1,
	DXT3,
	DXT5,
	ETC1,
	PVRTC_RGB_2BPP,
	PVRTC_RGBA_2BPP,
	PVRTC_RGB_4BPP,
	PVRTC_RGBA_4BPP,
};

/// Compressed image mip level
struct CompressedLevel
{
	/// Decompress to RGBA. The destination buffer required is width * height * 4 bytes. Return true if successful.
	bool Decompress(unsigned char* dest);

	/// Compressed image data.
	unsigned char* data_{};
	/// Compression format.
	CompressedFormat format_{ CompressedFormat::NONE };
	/// Width.
	int width_{};
	/// Height.
	int height_{};
	/// Depth.
	int depth_{};
	/// Block size in bytes.
	unsigned blockSize_{};
	/// Total data size in bytes.
	unsigned dataSize_{};
	/// Row size in bytes.
	unsigned rowSize_{};
	/// Number of rows.
	unsigned rows_{};
};

class Image : public Resource
{
	REGISTER_DERIVED_CLASS(Image, Resource);
	DECLARE_OBJECT_SERIALIZE(Image)
public:
	explicit Image(Ambient* ambient);
	/**
	* @brief : Register object factory
	*/
	static void RegisterObject(Ambient* ambient);
	/*
	*@brief : Load resource from stream.May be called from a worker thread.Return true if successful
	*/
	bool BeginLoad(Deserializer& source)override;
	/**
	* @brief : Save resource. Return true if successful
	*/
	bool Save(Serializer& dest) const override;
	/**
	* @brief : Set 2D size and number of color components. 
	*	Old image data will be destroyed and new data is undefined.
	*	Return true if successful.
	*/
	bool SetSize(int width, int height, unsigned components);
	/**
	* @brief : Set 3D size and number of color components.
	*	Old image data will be destroyed and new data is undefined. 
	*	Return true if successful.
	*/
	bool SetSize(int width, int height, int depth, unsigned components);
	/**
	* @brief : Set new image data
	*/
	void SetData(const unsigned char* pixelData);

	/**
	* @brief : Return whether is compressed
	*/
	bool IsCompressed() const { return _compressedFormat != CompressedFormat::NONE; }
	/**
	* @brief : Return compressed format
	*/
	CompressedFormat GetCompressedFormat() const { return _compressedFormat; }

private:
	/**
	* @brief : Decode an image using stb_image
	*/
	static unsigned char* getImageData(Deserializer& source, int& width, int& height, unsigned& components);
	/**
	* @brief : Free an image file's pixel data
	*/
	static void freeImageData(unsigned char* pixelData);

private:
	/// Width
	int _width{};
	/// Height
	int _height{};
	/// Depth
	int _depth{};
	/// Number of color components
	unsigned _components{};
	/// Number of compressed mip levels
	unsigned _numCompressedLevels{};
	/// Cubemap status if DDS
	bool _cubemap{};
	/// Texture array status if DDS
	bool _array{};
	/// Data is sRGB
	bool _sRGB{};
	/// Compressed format
	CompressedFormat _compressedFormat{ CompressedFormat::NONE };
	/// Pixel data
	SharedArrayPtr<unsigned char> _data;
	/// Precalculated mip level image
	SharedPtr<Image> _nextLevel;
	/// Next texture array or cube map image
	SharedPtr<Image> _nextSibling;
};

}