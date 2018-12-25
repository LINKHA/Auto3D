#include "Component.h"
#include "Scene.h"
#include "NewDef.h"

namespace Auto3D {

Component::Component(Ambient* ambient)
	: Super(ambient)
	, _isEnable(true)
{}

Component::~Component() {}

Node& Component::GetNode()
{
	return *_node;
}

const Node& Component::GetNode() const
{
	return *_node;
}

Node* Component::GetNodePtr()
{
	return _node;
}

Node* Component::GetNodePtr() const
{
	return _node;
}

void Component::SetNode(Node* node)
{
	_node = node;
}

Vector3 Component::GetPosition()
{
	if(!GetNodePtr())
		return Vector3();
	return GetNodePtr()->GetPosition();
}

const int Component::GetSceneID()
{
	return GetNode().GetSceneID();
}

SceneNode* Component::GetCurrentSceneNode()
{
	return GetSubSystem<Scene>()->GetLevelScene(GetSceneID())->GetSceneNode();
}

}