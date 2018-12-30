#include "SceneNode.h"
#include "PhysicsWorld.h"
#include "PhysicsWorld2D.h"
#include "NewDef.h"

namespace Auto3D {

SceneNode::SceneNode(SharedPtr<Ambient> ambient,int id)
	: Super(ambient, id)
{
	//_physcisWorld2D = CreateComponent<PhysicsWorld2D>();
	//_physcisWorld = CreateComponent<PhysicsWorld>();
}


SceneNode::~SceneNode()
{
}

SharedPtr<PhysicsWorld> SceneNode::GetPhysicsWorld()
{ 
	return GetComponent<PhysicsWorld>(); }

SharedPtr<PhysicsWorld2D> SceneNode::GetPhysicsWorld2D()
{ 
	return GetComponent<PhysicsWorld2D>(); 
}

}