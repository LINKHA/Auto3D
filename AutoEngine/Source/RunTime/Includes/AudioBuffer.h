#pragma once
#include "ComponentSetting.h"
namespace Auto3D {

class AudioBuffer : public ComponentSetting
{
	REGISTER_OBJECT_CLASS(AudioBuffer, ComponentSetting)
public:
	explicit AudioBuffer(Ambient* ambient,STRING path);
	/**
	* @brief : Get audio clip data
	*/
	STRING GetData() const { return _data; }
private:
	STRING _data;
};

}