#pragma once
#include "ComponentSetting.h"
namespace Auto3D {
class HDR : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(HDR, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(HDR);
public:
	explicit HDR(Ambient* ambient);

};

}