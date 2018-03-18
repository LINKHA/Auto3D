#ifndef SCRIPT_MANAGER_H_
#define SCRIPT_MANAGER_H_
#include "Auto.h"
#include "config.h"
#include "stl_use.h"
#include "BaseScript.h"
SCRIPT_BEGIN
class BaseScript;

class ScriptManager
{
public:

	ScriptManager();
	~ScriptManager();


	BaseScript* FindScript(const std::string& className);




};
SCRIPT_END
#endif // !SCRIPT_MANAGER_H_
