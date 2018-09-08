#include "Node.h"
#include "GameObject.h"


namespace Auto3D {

Node::Node(Ambient* ambient, int levelBumber)
	: Super(ambient)
	, _levelBumber(levelBumber)
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

Node::GameObjectChilds& Node::GetAllChild()
{
	return _childs;
}

}