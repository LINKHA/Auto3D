#include "ScriptManager.h"

SCRIPT_BEGIN
struct MatchByNameFilter
{
	const char* name;
	bool Match(BaseScript* script) { return script->GetScriptClassName() == name; }
};

struct MatchByClassFilter
{
	//ScriptingClassPtr m_class;
	//bool Match(BaseScript* script) { return script->GetClass() == m_class; }
};


template<typename T>
BaseScript* FindScript(ScriptManager::Scripts& scripts , T& filter)
{
	ScriptManager::Scripts::iterator i, next;
	for (i = scripts.begin(); i != scripts.end(); i = next)
	{
		next = i; next++;
		BaseScript* script = *i;
		if (script == NULL)
		{
			scripts.erase(i);
		}
		else if(filter.Match(script))
		{
			return script;
		}
	}
	return NULL;
}
SCRIPT_END