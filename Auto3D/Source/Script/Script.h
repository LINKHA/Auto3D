#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

/// Script sub system 
class AUTO_API Script : public Subsystem
{
	REGISTER_OBJECT_CLASS(Script, Subsystem)
public:
	/// Construct
	Script();

	~Script();

};


}