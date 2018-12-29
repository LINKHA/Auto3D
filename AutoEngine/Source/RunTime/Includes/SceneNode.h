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
	SharedPtr<PhysicsWorld> GetPhysicsWorld() { return _physcisWorld; }
	/**
	* @brief : Get 2D physics world
	*/
	SharedPtr<PhysicsWorld2D> GetPhysicsWorld2D() { return _physcisWorld2D; }
private:
	/// scene ID
	int _levelID{};
	/// 2D physcis world
	SharedPtr<PhysicsWorld> _physcisWorld;
	/// 3D physcis world
	SharedPtr<PhysicsWorld2D> _physcisWorld2D;
};

}