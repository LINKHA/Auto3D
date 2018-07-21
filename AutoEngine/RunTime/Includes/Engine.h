#pragma once
#include "Object.h"
AUTO_BEGIN
class Engine : public Object
{
	REGISTER_DERIVED_CLASS(Engine, Object);
	DECLARE_OBJECT_SERIALIZE(Engine);
public:
	explicit Engine(Ambient* ambient);

};

AUTO_END