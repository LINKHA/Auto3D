#pragma once
#include "Object.h"


namespace Auto3D {

class Scene : public Object
{
	REGISTER_OBJECT_CLASS(Scene, Object)
public:
	explicit Scene(SharedPtr<Ambient> ambient);

};

}