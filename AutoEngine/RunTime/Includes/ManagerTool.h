#pragma once
#include "Object.h"
namespace Auto3D {

class ManagerTool : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ManagerTool, Object);
	DECLARE_OBJECT_SERIALIZE(ManagerTool);
public:
	explicit ManagerTool(Ambient* ambient);

};

}