#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

/// Script sub system 
class AUTO_API Script : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(Script, BaseSubsystem)
public:
	/// Construct
	Script();

	~Script();

};


}