#pragma once
#include "Object.h"

namespace Auto3D {

class Constraint2D : public Object
{
	REGISTER_OBJECT_CLASS(Constraint2D, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit Constraint2D(SharedPtr<Ambient> ambient);
};

}