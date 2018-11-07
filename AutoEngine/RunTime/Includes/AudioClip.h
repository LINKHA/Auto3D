#pragma once
#include "ComponentSetting.h"
namespace Auto3D {

class AudioClip : public ComponentSetting
{
public:
	REGISTER_DERIVED_CLASS(AudioClip, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(AudioClip);
public:
	explicit AudioClip(Ambient* ambient,STRING path);
	STRING GetPath() const { return _path; }
private:
	STRING _path;
};

}