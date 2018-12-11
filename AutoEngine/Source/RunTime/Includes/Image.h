#pragma once
#include "Resource.h"

namespace Auto3D {

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

/**
* Compressed image mip level
*/
struct CompressedLevel
{
	/// decompress to RGBA. The destination buffer required is width * height * 4 bytes. Return true if successful
	bool Decompress(unsigned char* dest);

	/// compressed image data
	unsigned char* _data{};
	/// compression format
	CompressedFormat _format{ CompressedFormat::NONE };
	/// width
	int _width{};
	/// height
	int _height{};
	/// depth
	int _depth{};
	/// block size in bytes
	unsigned _blockSize{};
	/// total data size in bytes
	unsigned _dataSize{};
	/// row size in bytes
	unsigned _rowSize{};
	/// number of rows
	unsigned _rows{};
};

enum class ImageFormat
{

};

enum class ImageType
{
	Opaque,
	Custom,
	Translucent,
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
	bool SetSize(int width, int height, unsigned comp);
	/**
	* @brief : Set 3D size and number of color components.
	*	Old image data will be destroyed and new data is undefined. 
	*	Return true if successful.
	*/
	bool SetSize(int width, int height, int depth, unsigned comp);
	/**
	* @brief : Set new image data
	*/
	void SetData(const unsigned char* pixelData);
	/**
	* @brief : Set format
	*/
	void SetFormat(unsigned comp);
	/**
	* @brief : Set image type
	*/
	void SetImageType(ImageType type);

	/**
	* @brief : Return width
	*/
	int GetWidth() const { return _width; }
	/**
	* @brief : Return height
	*/
	int GetHeight() const { return _height; }
	/**
	* @brief : Return depth
	*/
	int GetDepth() const { return _depth; }
	/**
	* @brief : Return format
	*/
	unsigned GetFormat() { return _format; }
	/**
	* @brief : Return number of color components
	*/
	unsigned GetComponents() const { return _components; }
	/**
	* @brief : Return pixel data
	*/
	unsigned char* GetData() const { return _data; }
	/**
	* @brief : Return image type
	*/
	ImageType GetType()const { return _imageType; }
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
	
public:
	/// width
	int _width{};
	/// height
	int _height{};
	/// depth
	int _depth{};
	/// number of color components
	unsigned _components{};
	/// format
	unsigned _format{};
	/// number of compressed mip levels
	unsigned _numCompressedLevels{};
	/// cubemap status if DDS
	bool _cubemap{};
	/// texture array status if DDS
	bool _array{};
	/// data is sRGB
	bool _sRGB{};
	/// compressed format
	CompressedFormat _compressedFormat{ CompressedFormat::NONE };
	/// image type
	ImageType _imageType{ ImageType::Opaque };
	/// pixel data
	unsigned char* _data;
	/// precalculated mip level image
	SharedPtr<Image> _nextLevel;
	/// next texture array or cube map image
	SharedPtr<Image> _nextSibling;
};

}