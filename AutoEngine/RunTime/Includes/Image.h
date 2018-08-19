#pragma once
#include "Auto.h"
#include "Math/Color.h"




namespace Auto3D {

class Image
{
public:
	Image();
	Image(int width, int height);
	~Image() = default;

	UInt8* GetImageData() const { return value; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetChannels() const { return channels; }

	UInt32 format;
	Int32 width;
	Int32 height;
	Int32 channels;
	unsigned char* value;

};


}
