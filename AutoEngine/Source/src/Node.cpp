#include "Node.h"
#include "Ambient.h"
#include "NewDef.h"

namespace Auto3D {

Node::Node(SharedPtr<Ambient> ambient)
	: Super(ambient),
	flags(NF_ENABLED),
	layer(LAYER_DEFAULT),
	tag(TAG_NONE),
	parent(nullptr),
	scene(nullptr),
	id(0)
{
}

Node::~Node() 
{
	
}

SharedPtr<Node> Node::CreateChild(STRING childType)
{
	SharedPtr<Object> newObject = _ambient->CreateObject(childType);
	if (!newObject)
	{
		ErrorString("Could not create child node of unknown type ");
		return nullptr;
	}
	SharedPtr<Node> child = DynamicCast<Node>(newObject);
	if (!child)
	{
		ErrorString("Is not a Node subclass, could not add as a child");
		return nullptr;
	}

	AddChild(child);
	return child;
}

SharedPtr<Node> Node::CreateChild(STRING childType, const STRING& childName)
{
	return CreateChild(childType, childName.CStr());
}

SharedPtr<Node> Node::CreateChild(STRING childType, const char* childName)
{
	SharedPtr<Node> child = CreateChild(childType);
	if (child)
		//child->SetName(childName);
	return child;
}


}