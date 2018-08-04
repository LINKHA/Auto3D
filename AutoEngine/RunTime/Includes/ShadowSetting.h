#pragma once
#include "Auto.h"
#include "ComponentSetting.h"

namespace Auto3D {

class ShadowSetting : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(ShadowSetting, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(ShadowSetting);
public:
	explicit ShadowSetting(Ambient* ambient);
};

}
