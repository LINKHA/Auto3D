#include "Node.h"
#include "GameObject.h"


namespace Auto3D {

Node::Node(Ambient* ambient, int levelBumber)
	: Super(ambient)
	, _levelBumber(levelBumber)
{}
Node::~Node() {}


void Node::AddChild(GameObject* node)
{
	_childs.PushBack(node);
}

void Node::RemoveChild(int index)
{
	if (_childs.Begin() + index <= _childs.End())
		_childs.Erase(_childs.Begin() + index);
	else
		ErrorString("File remove child,the index is exceed child count.\n");
}

GameObject* Node::GetChild(int index)
{
	if (_childs.Begin() + index >= _childs.End())
	{
		ErrorString("File get child,the index is exceed child count.\n");
		return _childs.At(_childs.Size());
	}
	return _childs.At(index);
}

Node::GameObjectChilds& Node::GetAllChild()
{
	return _childs;
}

}