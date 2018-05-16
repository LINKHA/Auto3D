#include "GameObject.h"


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
{
}

void Component::MountComponent(GameObject* gameObject, Component& com)
{
	m_GameObject = gameObject;
	//m_GameObject.addComponent(com);
}
void Component::MountComponent(GameObject* gameObject, Component* com)
{
	m_GameObject = gameObject;
	//m_GameObject.addComponent(com);
}



//////////////////////////////////////////////////////////////////////////
//GameObject
//////////////////////////////////////////////////////////////////////////
GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}




AUTO_END


