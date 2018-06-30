#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_
#include "config.h"
#include "stl_use.h"
#include "ScriptComponent.h"
#include "GameManager.h"
#include "Singleton.h"
AUTO_BEGIN
class ScriptComponent;

class ScriptManager : public GlobalGameManager , Singleton<ScriptManager>
{
	REGISTER_DERIVED_CLASS(ScriptManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(ScriptManager);
public:
	typedef _SET(ScriptComponent) Scripts;
	typedef _SET(ScriptComponent*) AllScript;

};
AUTO_END
#endif // !SCRIPT_MANAGER_H_
