#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Ambient.h"

namespace Auto3D {
//////////////////////////////////////////////////////////////////////////
//Node
//////////////////////////////////////////////////////////////////////////
Node::Node(Ambient* ambient)
	:Super(ambient)
{}
Node::~Node() {}
void Node::AddChild(const GameObject& node)
{
	_childs.push_back(node);
}
void Node::RemoveChild(int index)
{
	if (_childs.begin() + index <= _childs.end())
		_childs.erase(_childs.begin() + index);
	else
		ErrorString("File remove child,the index is exceed child count.\n");
}
GameObject& Node::GetChild(int index)
{
	if (_childs.begin() + index >= _childs.end())
	{
		ErrorString("File get child,the index is exceed child count.\n");
		return _childs.at(_childs.size());
	}
	return _childs.at(index);
}
Node::GameObjectNodeArray& Node::GetAllChild()
{
	return _childs;
}

//////////////////////////////////////////////////////////////////////////
//Component
//////////////////////////////////////////////////////////////////////////
Component::Component()
	: _isEnable(true)
{}
Component::Component(Ambient* ambient)
	: Super(ambient)
	, _isEnable(true)
{}
Component::~Component(){}
GameObject& Component::GetGameObject()
{
	return *_gameObject.ptr;
}
const GameObject& Component::GetGameObject() const
{
	return *_gameObject.ptr;
}
GameObject* Component::GetGameObjectPtr()
{
	return _gameObject.ptr;
}
GameObject* Component::GetGameObjectPtr() const
{
	return _gameObject.ptr;
}

void Component::MountComponent(GameObject& gameObject)
{
	_gameObject.ptr = &gameObject;
}
//////////////////////////////////////////////////////////////////////////
//GameObject
//////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
	:_isEnable(true)
{
	AddComponent(new Transform());
	INSTANCE(GameObjectManager).AddGameObject(this);
}
GameObject::GameObject(Ambient* ambient)
	: Super(ambient)
	,_isEnable(true)
{
	AddComponent(new Transform());
	INSTANCE(GameObjectManager).AddGameObject(this);
}
GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component* com)
{
	_components.push_back(M_PAIR(com->GetClassIDVirtual(), com));
	com->MountComponent(*this);
}
void GameObject::RemoveComponentAtIndex(int index)
{
	ComponentsArray::iterator it = _components.begin() + index;
	_components.erase(it);
}

int GameObject::GetComponentSize() 
{ 
	return static_cast<int>(_components.size());
}
const GameObject& GameObject::GetGameObject()const 
{ 
	return *this; 
}
GameObject& GameObject::GetGameObject() 
{ 
	return *this; 
}
Component* GameObject::QueryComponent(int classID) const
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		if (it->first == classID)
			return it->second;
	}
	ErrorString("File find component of ClassId.");
	return nullptr;
}
}