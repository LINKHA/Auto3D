#pragma once
#include "Auto.h"
#include "Math/Color.h"




namespace Auto3D {

enum class ClearMode
{
	kColor = 1,
	kAlpha = 2,
	kColorAlpha = kColor | kAlpha
};

enum class BlitMode
{
	kCopy,
	kScale,
	kBilinearScale
};

class ImageReference
{
public:
	UInt32	format;
	Int32	width;
	Int32	height;
	Int32   channels;
	unsigned char *	value;
	

	ImageReference() { value = NULL; width = 0; height = 0; format = 0; }
	//ImageReference(int width, int height, int rowbytes, TextureFormat format, void* image);
	//ImageReference(int width, int height, TextureFormat format);

	// Returns true if the image is exactly the same by camping width, height, and image data
	friend bool operator == (const ImageReference& lhs, const ImageReference& rhs);

	// Returns a subpart of the image
	ImageReference ClipImage(int x, int y, int width, int height) const;

	UInt8* GetImageData() const { return value; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetChannels() const { return channels; }
	//TextureFormat GetFormat() const { return (TextureFormat)m_Format; }
	void BlitImage(const ImageReference& source, BlitMode mode = BlitMode::kCopy);
	void BlitImage(int x, int y, const ImageReference& source);
	void ClearImage(const Color& color, ClearMode mode = ClearMode::kColorAlpha);
	void FlipImageY();
	bool IsValidImage() const;
};



class Image : public ImageReference 
{
public:
	Image();
	Image(int width, int height);
	~Image();
};
}
