#pragma once
#include "Object.h"

namespace Auto3D {

class Constraint : public Object
{
	REGISTER_OBJECT_CLASS(Constraint, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit Constraint(SharedPtr<Ambient> ambient);
};

}