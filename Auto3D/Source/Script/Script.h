#pragma once
#include "Core/GameManager.h"

namespace Auto3D {

/// Script sub system 
class AUTO_API Script : public ABaseModule
{
	REGISTER_OBJECT_CLASS(Script, ABaseModule)
public:
	/// Construct
	Script();

	~Script();

};


}