#pragma once
#include "Component.h"
#include "AutoPhysics2D.h"

namespace Auto3D {

class RigidBody2D;

class PhysicsWorld2D : public Component//, public b2ContactListener, public b2Draw
{
	REGISTER_OBJECT_CLASS(PhysicsWorld2D, Component)
public:
	explicit PhysicsWorld2D(SharedPtr<Ambient> ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(SharedPtr<Ambient> ambient);

	void Update()override;
	/**
	* @brief : Return the Box2D physics world
	*/
	b2World* GetWorld() { return _world; }
	/**
	* @brief : Add rigid body
	*/
	void AddRigidBody(SharedPtr<RigidBody2D> rigidBody);
	/**
	* @brief : Remove rigid body
	*/
	void RemoveRigidBody(SharedPtr<RigidBody2D> rigidBody);


private:
	b2World* _world;
	/// Is phtsics stepping
	bool _isPhysicsStepping{};
	/// Gravity default -9.81
	Vector2 _gravity;
	/// Velocity iterations default 8
	int _velocityIter{};
	/// Position iterations default 3
	int _positionIter{};

	VECTOR<SharedPtr<RigidBody2D> > _rigidBodys;
};

}