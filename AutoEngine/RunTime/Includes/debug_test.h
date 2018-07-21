#pragma once
#include "GameObject.h"
AUTO_BEGIN
class debug_test : public Component
{
	REGISTER_DERIVED_CLASS(debug_test, Component);
	DECLARE_OBJECT_SERIALIZE(debug_test);
public:
	debug_test();
	void Start()override;
};

AUTO_END	