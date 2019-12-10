#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D {

	class APhysicsWorld2D;

/// Rigid body type.
namespace EBodyType2D
{
	enum Type
	{
		STATIC = b2_staticBody,
		KINEMATIC = b2_kinematicBody,
		DYNAMIC = b2_dynamicBody
	};
}

class AUTO_API ARigidBody2D : public ANode2D
{
	DECLARE_CLASS(ARigidBody2D, ANode2D)
public:
	/// Construct
	ARigidBody2D();
	/// Destructor
	~ARigidBody2D();
	/// Register object factory.
	static void RegisterObject();
	/// Get box2d body.
	b2Body* GetBody() { return _body; }

	/// Set body type.
	void SetBodyType(EBodyType2D::Type type);
	/// Set mass.
	void SetMass(float mass);
	/// Set inertia.
	void SetInertia(float inertia);
	/// Set mass center.
	void SetMassCenter(const TVector2F& center);
	/// Return whether to calculate mass and inertia from collision shapes automatically.
	bool GetUseFixtureMass() const { return _useFixtureMass; }
	/// Apply world transform from the Box2D body. Called by APhysicsWorld2D.
	void ApplyWorldTransform();
	/// Apply specified world position & rotation. Called by APhysicsWorld2D.
	void ApplyWorldTransform(const TVector3F& newWorldPosition,const FQuaternion& newWorldRotation);

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack()override;
private:
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();
	/// Remove the rigid body from the physics world.
	void RemoveBodyFromWorld();

	/// APhysics world 2d.
	TWeakPtr<APhysicsWorld2D> _physicsWorld2d;
	/// Box2D body define.
	b2BodyDef _bodyDef;
	/// Box2D body.
	b2Body* _body;
	/// Box2D mass data.
	b2MassData _massData;
	/// Use fixture mass (calculate mass & inertia from collision shapes automatically.)
	bool _useFixtureMass;
};

}