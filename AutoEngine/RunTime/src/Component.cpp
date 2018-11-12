#include "Component.h"
#include "GameObject.h"

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

}