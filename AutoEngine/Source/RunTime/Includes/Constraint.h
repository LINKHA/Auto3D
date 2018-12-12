#pragma once
#include "ComponentSetting.h"

namespace Auto3D {

class Constraint : public ComponentSetting
{
	REGISTER_OBJECT_CLASS(Constraint, ComponentSetting)
public:
	explicit Constraint(Ambient* ambient);
	

};

}