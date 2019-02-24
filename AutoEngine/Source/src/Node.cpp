#include "Node.h"
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


}