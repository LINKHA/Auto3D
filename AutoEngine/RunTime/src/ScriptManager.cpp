#include "ScriptManager.h"

AUTO_BEGIN

struct MatchByNameFilter
{
	const char* name;
	bool Match(ScriptComponent* script) { return script->GetClassStringStatic() == name; }
};


SINGLETON_INSTANCE(ScriptComponent);
template<typename T>
ScriptComponent* FindScript(ScriptManager::Scripts& scripts , T& filter)
{
	ScriptManager::Scripts::iterator i, next;
	for (i = scripts.begin(); i != scripts.end(); i = next)
	{
		next = i; next++;
		ScriptComponent* script = *i;
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
AUTO_END