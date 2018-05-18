#ifndef BASE_SCRIPT_H_
#define BASE_SCRIPT_H_

#include "Auto.h"
#include "BaseObject.h"

SCRIPT_BEGIN
USING_AUTO

class BaseScript
{
public:
	BaseScript();
	~BaseScript();
	virtual const _String GetScriptClassName() const { return m_ClassName; }
	virtual const void SetScriptClassName(_String name) { m_ClassName = name; }
	
private:

	const BaseScript* m_ScriptCache;

	_String m_ClassName;
};

SCRIPT_END
#endif // !BASE_SCRIPT_H_

