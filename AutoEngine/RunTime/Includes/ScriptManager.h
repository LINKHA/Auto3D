#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_
#include "config.h"
#include "stl_use.h"
#include "BaseScript.h"
#include "BaseObject.h"


SCRIPT_BEGIN
USING_AUTO
class BaseScript;

class ScriptManager
{
public:

	typedef _SET(PPtr<BaseScript>) Scripts;
	typedef _SET(BaseScript*) AllScript;

};
SCRIPT_END
#endif // !SCRIPT_MANAGER_H_
