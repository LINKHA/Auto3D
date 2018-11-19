#include "SceneNode.h"
#include "PhysicsWorld.h"
#include "PhysicsWorld2D.h"

namespace Auto3D {

SceneNode::SceneNode(Ambient* ambient,int id)
	: Super(ambient, id)
{
	_physcisWorld2D = CreateComponent<PhysicsWorld2D>();
	_physcisWorld = CreateComponent<PhysicsWorld>();
}


SceneNode::~SceneNode()
{
}

}