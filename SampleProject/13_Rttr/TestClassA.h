#pragma once
#include "Adapter/AutoRttr.h"
#include "Container/String.h"
#include "Container/Ptr.h"

using namespace Auto3D;

namespace Ns3D
{

class Node : public FRefCounted
{
	RTTR_ENABLE()
	RTTR_REGISTRATION_FRIEND
public:
	Node(FString name, Node* parent = nullptr):
		_name(name),
		_parent(parent)
	{}
	virtual ~Node() {}
	void SetName(const FString& name)
	{
		_name = name;
	}
	const FString& GetName() const
	{
		return _name;
	}
	std::vector<Node*> GetChildren() const
	{
		return _children;
	}
	void SetVisible(bool visible, bool cascade = true) {}
	virtual void Render() {}
protected:
	Node* _parent;
	FString _name;
	std::vector<Node*> _children;
	
};

}