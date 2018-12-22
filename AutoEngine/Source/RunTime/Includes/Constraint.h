#pragma once
#include "Object.h"

namespace Auto3D {

class Constraint : public Object
{
	REGISTER_OBJECT_CLASS(Constraint, Object)
public:
	explicit Constraint(Ambient* ambient);


};

}