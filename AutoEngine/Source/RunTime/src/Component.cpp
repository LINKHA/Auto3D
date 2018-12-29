#include "Component.h"
#include "Scene.h"
#include "NewDef.h"

namespace Auto3D {

Component::Component(Ambient* ambient)
	: Super(ambient)
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

SharedPtr<Node> Component::GetNodePtr()
{
	return _node;
}

SharedPtr<Node> Component::GetNodePtr() const
{
	return _node;
}

void Component::SetNode(SharedPtr<Node> node)
{
	_node = node;
}

Vector3 Component::GetPosition()
{
	if(!GetNodePtr())
		return Vector3();
	return GetNodePtr()->GetPosition();
}

const int Component::GetLevelID()
{
	return GetNode().GetLevelID();
}

SharedPtr<SceneNode> Component::GetCurrentSceneNode()
{
	return GetSubSystem<Scene>()->GetLevelScene(GetLevelID())->GetSceneNode();
}

}