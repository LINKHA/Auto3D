#pragma once
#include "ComponentSetting.h"
namespace Auto3D {

class AudioBuffer : public ComponentSetting
{
public:
	REGISTER_DERIVED_CLASS(AudioBuffer, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(AudioBuffer);
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