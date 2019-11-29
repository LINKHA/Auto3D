#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D{
	
class RigidBody2D;
class PhysicsWorld2D;

class AUTO_API Collider2D : public Node2D
{
	REGISTER_OBJECT_CLASS(Collider2D,Node2D)
public:
	/// Construct
	Collider2D();
	/// Destructor
	virtual ~Collider2D();
	/// Register object factory.
	static void RegisterObject();
	/// Set density.
	void SetDensity(float density);
	/// Set friction.
	void SetFriction(float friction);
	/// Set restitution .
	void SetRestitution(float restitution);
	/// Update the new collision shape to the RigidBody.
	void CreateFixture();
	/// Release fixture.
	void ReleaseFixture();
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;
protected:
	/// Physics world form this collider
	TWeakPtr<PhysicsWorld2D> _physicsWorld;
	/// Rigid body.
	TWeakPtr<RigidBody2D> _rigidBody;
	/// Fixture def.
	b2FixtureDef _fixtureDef;
	///// Box2D fixture.
	b2Fixture* _fixture;
};

}

