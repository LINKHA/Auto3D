#include "Node.h"
#include "Component.h"
#include "Transform.h"
#include "Scene.h"
#include "Ambient.h"

namespace Auto3D {

Node::Node(Ambient* ambient, int levelBumber)
	: Super(ambient)
	, _levelBumber(levelBumber)
{
	AddComponent(_ambient->CreateObject<Transform>());
	// add node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_levelBumber)->AddNode(this);
}
Node::~Node() 
{
	// remove node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_levelBumber)->RemoveNode(this);
}


void Node::AddChild(Node* node)
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

Node* Node::GetChild(int index)
{
	if (_childs.begin() + index >= _childs.end())
	{
		ErrorString("File get child,the index is exceed child count.\n");
		return _childs.at(_childs.size());
	}
	return _childs.at(index);
}

Node::GameObjectChilds& Node::GetAllChild()
{
	return _childs;
}

void Node::AddComponent(Component* com)
{
	_components.push_back(MAKE_PAIR(com->GetClassID(), com));
	com->MountComponent(*this);
}

void Node::RemoveComponentAtIndex(int index)
{
	ComponentsArray::iterator it = _components.begin() + index;
	_components.erase(it);
}

int Node::GetComponentsSize()
{
	return static_cast<int>(_components.size());
}

const Node& Node::GetGameObject()const
{
	return *this;
}

Node& Node::GetGameObject()
{
	return *this;
}

Component* Node::QueryComponent(int classID) const
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		if (it->first == classID)
			return it->second;
	}
	ErrorString("Fail find component of ClassId.");
	return nullptr;
}

Vector3 Node::GetPosition()
{
	return GetComponent(Transform).GetPosition();
}

}