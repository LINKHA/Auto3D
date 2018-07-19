#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform.h"
AUTO_BEGIN
//////////////////////////////////////////////////////////////////////////
//Node
//////////////////////////////////////////////////////////////////////////
Node::Node() {}
Node::~Node() {}
void Node::AddChild(const GameObject& node)
{
	m_Childs.push_back(node);
}
void Node::RemoveChild(arrayIndex index)
{
	if (m_Childs.begin() + index <= m_Childs.end())
		m_Childs.erase(m_Childs.begin() + index);
	else
		ErrorString("File remove child,the index is exceed child count.\n");
}
GameObject Node::GetChild(arrayIndex index)
{
	if (m_Childs.begin() + index >= m_Childs.end())
	{
		ErrorString("File get child,the index is exceed child count.\n");
		return m_Childs.at(m_Childs.size());
	}
	return m_Childs.at(index);
}
Node::GameObjectNodeArray Node::GetAllChild()
{
	return m_Childs;
}

//////////////////////////////////////////////////////////////////////////
//Component
//////////////////////////////////////////////////////////////////////////
Component::Component()
	: m_Enable(true)
{}
Component::~Component(){}
GameObject& Component::GetGameObject()
{
	return *m_gameObject.ptr;
}
const GameObject& Component::GetGameObject() const
{
	return *m_gameObject.ptr;
}
GameObject* Component::GetGameObjectPtr()
{
	return m_gameObject.ptr;
}
GameObject* Component::GetGameObjectPtr() const
{
	return m_gameObject.ptr;
}

void Component::MountComponent(GameObject& gameObject)
{
	m_gameObject.ptr = &gameObject;
}
//////////////////////////////////////////////////////////////////////////
//GameObject
//////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
	: m_Enable(true)
{
	AddComponent(new Transform());
	INSTANCE(GameObjectManager).AddGameObject(this);
}
GameObject::GameObject(Transform* transform)
	: m_Enable(true)
{
	AddComponent(transform);
	INSTANCE(GameObjectManager).AddGameObject(this);
}
GameObject::~GameObject()
{
}

void GameObject::AddComponent(Component* com)
{
	m_Components.push_back(M_PAIR(com->GetClassID(), com));
	com->MountComponent(*this);
}
void GameObject::RemoveComponentAtIndex(int index)
{
	ComponentsArray::iterator it = m_Components.begin() + index;
	m_Components.erase(it);
}

int GameObject::GetComponentSize() 
{ 
	return (int)m_Components.size(); 
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
	for (auto it = m_Components.begin(); it != m_Components.end(); it++)
	{
		if (it->first == classID)
			return it->second;
	}
	ErrorString("File find component of ClassId.");
	return nullptr;
}
AUTO_END