#include "Component.h"
#include "NewDef.h"

namespace Auto3D {

Component::Component(SharedPtr<Ambient> ambient)
	: Super(ambient)
{}

Component::~Component() {}

SharedPtr<Node> Component::GetNode()
{
	SharedPtr<Node> node;
	if (node = _node.lock())
		return node;
	else
	{
		ErrorString("Fail to get node from component");
		return SharedPtr<Node>();
	}
}

SharedPtr<Node> Component::GetNode() const
{
	SharedPtr<Node> node;
	if (node = _node.lock())
		return node;
	else
	{
		ErrorString("Fail to get node from component");
		return SharedPtr<Node>();
	}
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
	//return GetSubSystem<tScene>()->GetLevelScene(GetLevelID())->GetSceneNode();
	return SharedPtr<Node>();
}

}