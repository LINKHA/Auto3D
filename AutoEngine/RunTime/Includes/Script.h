#pragma once
#include "stl_use.h"
#include "GameManager.h"

namespace Auto3D {
class ScriptComponent;

class Script : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Script, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Script);
	using Scripts = _SET(ScriptComponent);
	using AllScript = _SET(ScriptComponent*);
public:
	explicit Script(Ambient* ambient);

};
}

