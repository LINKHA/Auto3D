#include "Component.h"
#include "GameObject.h"

namespace Auto3D {

Component::Component(Ambient* ambient)
	: Super(ambient)
	, _isEnable(true)
{}

Component::~Component() {}

GameObject& Component::GetGameObject()
{
	return *_gameObject;
}

const GameObject& Component::GetGameObject() const
{
	return *_gameObject;
}

GameObject* Component::GetGameObjectPtr()
{
	return _gameObject;
}

GameObject* Component::GetGameObjectPtr() const
{
	return _gameObject;
}

void Component::MountComponent(GameObject& gameObject)
{
	_gameObject = &gameObject;
}

}