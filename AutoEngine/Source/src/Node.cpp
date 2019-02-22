#include "Node.h"
#include "Component.h"
#include "Transform.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

Node::Node(SharedPtr<Ambient> ambient, int sceneID)
	: Super(ambient)
	, _levelID(sceneID)
	, _isEnable(true)
	, _name("default node")
{
}

Node::~Node() 
{
	_components.clear();
}

void Node::Destory()
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		it->second->Destory();
	}
}

void Node::AddChild(SharedPtr<Node> node)
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

SharedPtr<Node> Node::GetChild(int index)
{
	if (_childs.begin() + index >= _childs.end())
	{
		ErrorString("File get child,the index is exceed child count.\n");
		return _childs.at(_childs.size());
	}
	return _childs.at(index);
}

VECTOR<SharedPtr<Node> >& Node::GetAllChild()
{
	return _childs;
}

void Node::AddComponent(SharedPtr<Component> com)
{
	_components.push_back(MakePair(com->GetClassString(), com));
	com->SetNode(SharedFromThis(Node));
	com->Init();
}

void Node::RemoveComponentAtIndex(int index)
{
	PAIR_VECTOR<STRING, SharedPtr<Component> >::iterator it = _components.begin() + index;
	it->second->Destory();
	_components.erase(it);
}

SharedPtr<Component> Node::GetComponentIndex(int index)
{
	return _components[index].second;
}

int Node::GetComponentsSize()
{
	return _components.size();
}

SharedPtr<Node> Node::GetNode()
{
	return SharedFromThis(Node);
}

void Node::SetPosition(const Vector2F& position)
{
	auto transform = GetComponent<Transform>();
	transform->SetPosition(position._x, position._y, transform->GetPosition()._z);
}

void Node::SetPosition(const Vector3F& position)
{
	GetComponent<Transform>()->SetPosition(position._x, position._y, position._z);
}

void Node::SetPosition(float x, float y)
{
	auto transform = GetComponent<Transform>();
	transform->SetPosition(x, y, transform->GetPosition()._z);
}

void Node::SetPosition(float x, float y, float z)
{
	GetComponent<Transform>()->SetPosition(x, y, z);
}

void Node::SetRotation(const QuaternionF& rotation)
{
	GetComponent<Transform>()->SetRotation(rotation);
}

void Node::SetRotation(const Vector3F& euler)
{
	GetComponent<Transform>()->SetRotation(euler);
}

void Node::SetRotation(float Angle, const Vector3F& axis)
{
	GetComponent<Transform>()->SetRotation(Angle, axis);
}

void Node::SetScale(float scale)
{
	GetComponent<Transform>()->SetScale(scale);
}

void Node::SetScale(float scaleX, float scaleY)
{
	auto transform = GetComponent<Transform>();
	transform->SetScale(scaleX, scaleY, transform->GetScale()._z);
}

void Node::SetScale(float scaleX, float scaleY, float scaleZ)
{
	GetComponent<Transform>()->SetScale(scaleX, scaleY, scaleZ);
}

void Node::SetScale(const Vector2F& scale)
{
	auto transform = GetComponent<Transform>();
	transform->SetScale(scale._x, scale._y, transform->GetScale()._z);
}

void Node::SetScale(const Vector3F& scale)
{
	GetComponent<Transform>()->SetScale(scale);
}

Vector3F Node::GetPosition()
{
	return GetComponent<Transform>()->GetPosition();
}

SharedPtr<Component> Node::GetComponent(STRING type)
{
	for (auto it = _components.begin(); it != _components.end(); it++)
	{
		if (it->first == type)
			return it->second;
	}
	ErrorString("Fail find component of class string.");
	return nullptr;
}

SharedPtr<Component> Node::CreateComponent(STRING type)
{
	// Check that creation succeeds and that the object in fact is a component
	SharedPtr<Component> newComponent = DynamicCast<Component>(_ambient->CreateObject(type));
	if (!newComponent)
	{
		WarningString("Could not create unknown component type " + type);
		return nullptr;
	}

	AddComponent(newComponent);
	return newComponent;
}

}