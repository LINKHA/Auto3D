#pragma once
#include "Object.h"
AUTO_BEGIN

class ManagerTool : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ManagerTool, Object);
	DECLARE_OBJECT_SERIALIZE(ManagerTool);
public:
	explicit ManagerTool(Ambient* ambient);

};

AUTO_END