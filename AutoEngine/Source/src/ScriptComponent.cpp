#include "ScriptComponent.h"
#include "Ambient.h"
#include "Transform.h"
#include "tScene.h"

#include "NewDef.h"

namespace Auto3D {
ScriptComponent::ScriptComponent(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}


ScriptComponent::~ScriptComponent()
{
}

SharedPtr<Node> ScriptComponent::CreateNode(STRING name)
{
	SharedPtr<Node> node = MakeShared<Node>(_ambient, _levelID);
	GetSubSystem<tScene>()->GetLevelScene(_levelID)->AddNode(node);
	node->SetName(name);
	//Each node must have a Transform component and cannot be deleted
	node->CreateComponent<Transform>();

	return node;
}


}