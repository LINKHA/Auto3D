#pragma once
#include "../Core/GameManager.h"

namespace Auto3D {

/// Script sub system 
class AUTO_API Script : public BaseModule
{
	REGISTER_OBJECT_CLASS(Script, BaseModule)
public:
	/// Construct
	Script();

	~Script();

};


}