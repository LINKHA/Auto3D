#include "ScriptComponent.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {
ScriptComponent::ScriptComponent(Ambient* ambient)
	:Super(ambient)
{
}


ScriptComponent::~ScriptComponent()
{
}

Node* ScriptComponent::CreateNode(STRING name)
{
	Node* node = new Node(_ambient, _levelID);
	node->SetName(name);
	return node;
}


}