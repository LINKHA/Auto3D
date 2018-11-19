#pragma once
#include "ComponentSetting.h"

namespace Auto3D {

class Constraint : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(Constraint, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(Constraint);
public:
	explicit Constraint(Ambient* ambient);
	

};

}