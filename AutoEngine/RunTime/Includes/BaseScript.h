#ifndef BASE_SCRIPT_H_
#define BASE_SCRIPT_H_

#include "Auto.h"
#include "BaseObject.h"

SCRIPT_BEGIN
USING_AUTO

class BaseScript : public Object
{
public:
	BaseScript();
	~BaseScript();
	virtual const AutoString GetScriptClassName() const { return m_ClassName; }
	virtual const void SetScriptClassName(AutoString name) { m_ClassName = name; }
	
private:

	const BaseScript* m_ScriptCache;

	AutoString m_ClassName;
};

SCRIPT_END
#endif // !BASE_SCRIPT_H_

