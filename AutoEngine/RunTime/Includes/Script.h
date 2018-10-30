#pragma once
#include "AutoSTL.h"
#include "GameManager.h"

namespace Auto3D {

class ScriptComponent;
class Script : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Script, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Script);
	using Scripts = SET(ScriptComponent);
	using AllScript = SET(ScriptComponent*);
public:
	explicit Script(Ambient* ambient);

};

}

