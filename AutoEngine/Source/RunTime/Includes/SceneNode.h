#pragma once
#include "Node.h"


namespace Auto3D {
class PhysicsWorld;
class PhysicsWorld2D;
class SceneNode;
using SceneObject = class SceneNode;

class SceneNode : public Node
{
	REGISTER_DERIVED_CLASS(SceneNode, Node);
	DECLARE_OBJECT_SERIALIZE(SceneNode);
public:
	explicit SceneNode(Ambient* ambient,int id);
	/**
	* @brief : Get scene ID
	*/
	const int GetID() const { return _sceneID; }
	/**
	* @brief : Get 3D physics world
	*/
	PhysicsWorld* GetPhysicsWorld() { return _physcisWorld; }
	/**
	* @brief : Get 2D physics world
	*/
	PhysicsWorld2D* GetPhysicsWorld2D() { return _physcisWorld2D; }
private:
	/// scene ID
	int _sceneID{};
	/// 2D physcis world
	PhysicsWorld* _physcisWorld;
	/// 3D physcis world
	PhysicsWorld2D* _physcisWorld2D;
};

}