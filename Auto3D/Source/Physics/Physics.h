#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

/// Physics sub system 
class AUTO_API Physics : public Subsystem
{
	REGISTER_OBJECT_CLASS(Physics, Subsystem)
public:
	/// Construct
	Physics();

	~Physics();

};


}