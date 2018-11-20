#include "Component.h"
#include "Scene.h"

namespace Auto3D {

Component::Component(Ambient* ambient)
	: Super(ambient)
	, _isEnable(true)
{}

Component::~Component() {}

Node& Component::GetNode()
{
	return *_gameObject;
}

const Node& Component::GetNode() const
{
	return *_gameObject;
}

Node* Component::GetNodePtr()
{
	return _gameObject;
}

Node* Component::GetNodePtr() const
{
	return _gameObject;
}

void Component::MountComponent(Node& gameObject)
{
	_gameObject = &gameObject;
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