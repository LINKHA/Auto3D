#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D{
	
class ARigidBody2D;
class APhysicsWorld2D;

class AUTO_API ACollider2D : public ANode2D
{
	DECLARE_CLASS(ACollider2D,ANode2D)
public:
	/// Construct
	ACollider2D();
	/// Destructor
	virtual ~ACollider2D();
	/// Register object factory.
	static void RegisterObject();
	/// Set density.
	void SetDensity(float density);
	/// Set friction.
	void SetFriction(float friction);
	/// Set restitution .
	void SetRestitution(float restitution);
	/// Update the new collision shape to the ARigidBody.
	void CreateFixture();
	/// Release fixture.
	void ReleaseFixture();
	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;
protected:
	/// APhysics world form this collider
	TWeakPtr<APhysicsWorld2D> _physicsWorld;
	/// Rigid body.
	TWeakPtr<ARigidBody2D> _rigidBody;
	/// Fixture def.
	b2FixtureDef _fixtureDef;
	///// Box2D fixture.
	b2Fixture* _fixture;
};

}

