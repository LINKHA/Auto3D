#ifndef SCRIPT_COMPONTENT_H_
#define SCRIPT_COMPONTENT_H_
#include "BaseCompontent.h"
#include "BaseScript.h"


USING_SCRIPT

AUTO_BEGIN
class ScriptCompontent :public BaseCompontent ,public BaseScript
{
public:
	ScriptCompontent();
	~ScriptCompontent();
};

AUTO_END

#endif // SCRIPT_COMPONTENT_H_