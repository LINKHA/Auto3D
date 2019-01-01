#include "Component.h"
#include "Scene.h"
#include "NewDef.h"

namespace Auto3D {

Component::Component(SharedPtr<Ambient> ambient)
	: Super(ambient)
{}

Component::~Component() {}

SharedPtr<Node> Component::GetNode()
{
	return _node;
}

SharedPtr<Node> Component::GetNode() const
{
	return _node;
}

void Component::SetNode(SharedPtr<Node> node)
{
	_node = node;
}

Vector3 Component::GetPosition()
{
	if(!GetNode())
		return Vector3();
	return GetNode()->GetPosition();
}

const int Component::GetLevelID()
{
	return GetNode()->GetLevelID();
}

SharedPtr<Node> Component::GetCurrentSceneNode()
{
	return GetSubSystem<Scene>()->GetLevelScene(GetLevelID())->GetSceneNode();
}

}