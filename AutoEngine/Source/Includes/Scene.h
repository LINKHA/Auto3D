#pragma once
#include "Node.h"


namespace Auto3D {

class Scene : public Node
{
	REGISTER_OBJECT_CLASS(Scene, Node)
public:
	explicit Scene(SharedPtr<Ambient> ambient);

};

}