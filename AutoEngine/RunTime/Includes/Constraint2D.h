#pragma once
#include "ComponentSetting.h"

namespace Auto3D {

class Constraint2D : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Constraint2D, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Constraint2D);
public:
	explicit Constraint2D(Ambient* ambient);

};

}