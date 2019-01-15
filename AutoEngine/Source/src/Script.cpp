#include "Script.h"
#include "ScriptComponent.h"
#include "NewDef.h"

namespace Auto3D {

struct MatchByNameFilter
{
	const char* name;
	bool Match(ScriptComponent* script) { return script->GetClassStringStatic() == name; }
};


template<typename T>
ScriptComponent* FindScript(SET<ScriptComponent>& scripts , T& filter)
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
/////////////////////////////////////////////////////////////////////////////////////////////
//Script
/////////////////////////////////////////////////////////////////////////////////////////////
Script::Script(SharedPtr<Ambient> ambient)
	:Super(ambient)
{

}

Script::~Script()
{

}



}