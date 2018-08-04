#pragma once
#include "Object.h"
namespace Auto3D {

class ComponentSetting : public Object
{
	REGISTER_DERIVED_ABSTRACT_CLASS(ComponentSetting, Object);
	DECLARE_OBJECT_SERIALIZE(ComponentSetting);
public:
	explicit ComponentSetting(Ambient* ambient);

};

}