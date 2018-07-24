#pragma once
#include "Object.h"
#include "Ambient.h"
AUTO_BEGIN
class Graphics : public Object
{
	REGISTER_DERIVED_CLASS(Graphics, Object);
	DECLARE_OBJECT_SERIALIZE(Graphics);
public:
	explicit Graphics(Ambient* ambient);
};

AUTO_END
