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

SharedPtr<Node> ScriptComponent::CreateNode(STRING name)
{
	SharedPtr<Node> node = MakeShared<Node>(_ambient, _levelID);
	node->SetName(name);
	return node;
}


}