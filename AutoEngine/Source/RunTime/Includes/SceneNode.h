#pragma once
#include "Node.h"
namespace Auto3D {
class PhysicsWorld;
class PhysicsWorld2D;
class SceneNode;
using SceneObject = class SceneNode;

class SceneNode : public Node
{
	REGISTER_OBJECT_CLASS(SceneNode, Node)
public:
	explicit SceneNode(SharedPtr<Ambient> ambient,int id);
	/**
	* @brief : Get scene ID
	*/
	const int GetID() const { return _levelID; }
	/**
	* @brief : Get 3D physics world
	*/
	SharedPtr<PhysicsWorld> GetPhysicsWorld();
	/**
	* @brief : Get 2D physics world
	*/
	SharedPtr<PhysicsWorld2D> GetPhysicsWorld2D();
private:
	/// scene ID
	int _levelID{};
};

}