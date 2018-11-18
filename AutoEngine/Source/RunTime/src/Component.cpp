#include "Component.h"
#include "GameObject.h"
#include "Scene.h"

namespace Auto3D {

Component::Component(Ambient* ambient)
	: Super(ambient)
	, _isEnable(true)
{}

Component::~Component() {}

Node& Component::GetGameObject()
{
	return *_gameObject;
}

const Node& Component::GetGameObject() const
{
	return *_gameObject;
}

Node* Component::GetGameObjectPtr()
{
	return _gameObject;
}

Node* Component::GetGameObjectPtr() const
{
	return _gameObject;
}

void Component::MountComponent(Node& gameObject)
{
	_gameObject = &gameObject;
}

Vector3 Component::GetPosition()
{
	if(!GetGameObjectPtr())
		return Vector3();
	return GetGameObjectPtr()->GetPosition();
}

const int Component::GetSceneID()
{
	return GetGameObject().GetSceneID();
}

SceneNode* Component::GetCurrentSceneNode()
{
	return GetSubSystem<Scene>()->GetLevelScene(GetSceneID())->GetSceneNode();
}

}