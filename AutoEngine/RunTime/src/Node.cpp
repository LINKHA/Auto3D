#include "Node.h"
#include "Component.h"
#include "Transform.h"
#include "Scene.h"
#include "Ambient.h"

namespace Auto3D {

Node::Node(Ambient* ambient, int sceneID)
	: Super(ambient)
	, _sceneID(sceneID)
	, _isEnable(true)
{
	AddComponent(_ambient->CreateObject<Transform>());
	// add node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_sceneID)->AddNode(this);
}
Node::~Node() 
{
	// remove node to appoint level scene
	GetSubSystem<Scene>()->GetLevelScene(_sceneID)->RemoveNode(this);
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
	_components.push_back(MAKE_PAIR(com->GetClassString(), com));
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

Vector3 Node::GetPosition()
{
	return GetComponent<Transform>()->GetPosition();
}

Component* Node::GetComponent(STRING type)
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		if (it->first == type)
			return it->second;
	}
	ErrorString("Fail find component of Class ID.");
	return nullptr;
}

Component* Node::CreateComponent(STRING type)
{
	// Check that creation succeeds and that the object in fact is a component
	Component* newComponent = dynamic_cast<Component*>(_ambient->CreateObject(type));
	if (!newComponent)
	{
		WarningString("Could not create unknown component type " + type);
		return nullptr;
	}

	AddComponent(newComponent);
	return newComponent;
}

}