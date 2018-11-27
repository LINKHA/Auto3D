#pragma once
#include "Auto.h"
#include "Math/Color.h"
#include "ComponentSetting.h"



namespace Auto3D {

class tImage : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(tImage, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(tImage);
public:
	tImage(Ambient* ambient);
	tImage(Ambient* ambien,int width, int height);

	UInt8* GetImageData() const { return value; }
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }
	int GetChannels() const { return channels; }

	UInt32 format;
	Int32 width;
	Int32 height;
	Int32 channels;
	unsigned char* value{};

};


}
