#pragma once
#include "Auto.h"
#include "Math/Color.h"
#include "ComponentSetting.h"



namespace Auto3D {

class Image : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Image, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Image);
public:
	Image(Ambient* ambient);
	Image(Ambient* ambien,int width, int height);

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
