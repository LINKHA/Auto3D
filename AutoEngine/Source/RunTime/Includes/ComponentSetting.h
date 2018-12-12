#pragma once
#include "Object.h"
namespace Auto3D {

class ComponentSetting : public Object
{
	REGISTER_OBJECT_ABSTRACT_CLASS(ComponentSetting, Object)
public:
	explicit ComponentSetting(Ambient* ambient);

};

}