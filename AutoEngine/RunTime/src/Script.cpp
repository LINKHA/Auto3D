#include "Script.h"
#include "ScriptComponent.h"
namespace Auto3D {

struct MatchByNameFilter
{
	const char* name;
	bool Match(ScriptComponent* script) { return script->GetClassStringStatic() == name; }
};


template<typename T>
ScriptComponent* FindScript(Script::Scripts& scripts , T& filter)
{
	ScriptManager::Scripts::Iterator i, next;
	for (i = scripts.Begin(); i != scripts.End(); i = next)
	{
		next = i; next++;
		ScriptComponent* script = *i;
		if (script == NULL)
		{
			scripts.Erase(i);
		}
		else if(filter.Match(script))
		{
			return script;
		}
	}
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////
//Script
/////////////////////////////////////////////////////////////////////////////////////////////
Script::Script(Ambient* ambient)
	:Super(ambient)
{

}

Script::~Script()
{

}



}