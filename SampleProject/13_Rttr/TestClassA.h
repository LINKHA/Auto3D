#pragma once
#include "Adapter/AutoRttr.h"
#include "Container/String.h"
#include <string.h>

using namespace Auto3D;

namespace Ns3D
{

class Node
{
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
public:
	Node(FString name, Node* parent = nullptr) {}
	virtual ~Node() {}
	void set_name(const FString& name)
	{
		_name = name;
	}
	const FString& get_name() const
	{
		return _name;
	}
	std::vector<Node*> get_children() const
	{
		return _children;
	}
	void set_visible(bool visible, bool cascade = true) {}
	virtual void render() {}
protected:
	Node* _parent;
	FString _name;
	std::vector<Node*> _children;
	
};

}