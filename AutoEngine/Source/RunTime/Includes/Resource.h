#pragma once
#include "Object.h"

namespace Auto3D {
/**
* Base of all reource
*/
class Resource : public Object
{
	REGISTER_DERIVED_CLASS(Resource, Object);
	DECLARE_OBJECT_SERIALIZE(Resource)
public:
	explicit Resource(Ambient* ambient);

};

}