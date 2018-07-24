#pragma once
#include "GameObject.h"
AUTO_BEGIN
class debug_test : public Component
{
	REGISTER_DERIVED_CLASS(debug_test, Component);
	DECLARE_OBJECT_SERIALIZE(debug_test);
public:
	explicit debug_test(Ambient* ambient);
	void Start()override;
};

AUTO_END	