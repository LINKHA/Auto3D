#pragma once
#include "config.h"
#include "stl_use.h"
#include "ScriptComponent.h"
#include "GameManager.h"
#include "Singleton.h"
namespace Auto3D {
class ScriptComponent;

class ScriptManager : public GlobalGameManager , Singleton<ScriptManager>
{
	REGISTER_DERIVED_CLASS(ScriptManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(ScriptManager);
public:
	using Scripts = _SET(ScriptComponent) ;
	using AllScript = _SET(ScriptComponent*);

};
}

