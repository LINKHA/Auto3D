#pragma once
#include "GameManager.h"

namespace Auto3D {

class ScriptComponent;
class Script : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Script, GlobalGameManager)
public:
	explicit Script(SharedPtr<Ambient> ambient);

};

}

