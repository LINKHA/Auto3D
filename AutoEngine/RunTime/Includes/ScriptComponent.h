#ifndef SCRIPT_COMPONENT_H_
#define SCRIPT_COMPONENT_H_
#include "BaseComponent.h"
#include "BaseScript.h"


USING_SCRIPT

AUTO_BEGIN
class ScriptComponent :public BaseComponent ,public BaseScript
{
public:
	ScriptComponent();
	~ScriptComponent();
};

AUTO_END

#endif // SCRIPT_COMPONENT_H_
