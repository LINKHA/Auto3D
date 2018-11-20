#pragma once
#include "GameManager.h"

namespace Auto3D {

class ScriptComponent;
class Script : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Script, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Script);
public:
	explicit Script(Ambient* ambient);

};

}

