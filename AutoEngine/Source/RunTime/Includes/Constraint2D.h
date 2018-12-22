#pragma once
#include "Object.h"

namespace Auto3D {

class Constraint2D : public Object
{
	REGISTER_OBJECT_CLASS(Constraint2D, Object)
public:
	explicit Constraint2D(Ambient* ambient);

};

}