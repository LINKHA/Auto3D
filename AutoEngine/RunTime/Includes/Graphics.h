#pragma once
#include "Object.h"
AUTO_BEGIN
class Graphics : public Object
{
	REGISTER_DERIVED_CLASS(Graphics, Object);
	DECLARE_OBJECT_SERIALIZE(Graphics);
public:
	Graphics();
};

AUTO_END
