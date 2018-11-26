#pragma once
#include "Object.h"

namespace Auto3D {
const static char* RESOURCE_ATTACH = "Resource_attach";
/**
* Base of all reource
*/
class Resource : public Object
{
	REGISTER_DERIVED_CLASS(Resource, Object);
	DECLARE_OBJECT_SERIALIZE(Resource)
public:
	explicit Resource(Ambient* ambient);
	
	bool Load();
};

}