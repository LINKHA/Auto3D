#pragma once
#include "Object.h"


namespace Auto3D {

class View : public Object
{
	REGISTER_OBJECT_CLASS(View, Object)
public:
	explicit View(SharedPtr<Ambient> ambient);
};

}