#pragma once
#include "Node2D.h"
#include <Box2D.h>

namespace Auto3D {

	class PhysicsWorld2D;

/// Rigid body type.
namespace BodyType2D
{
	enum Type
	{
		STATIC = b2_staticBody,
		KINEMATIC = b2_kinematicBody,
		DYNAMIC = b2_dynamicBody
	};
}

class AUTO_API RigidBody2D : public Node2D
{
	REGISTER_OBJECT_CLASS(RigidBody2D, Node2D)
public:
	/// Construct
	RigidBody2D();
	/// Destructor
	~RigidBody2D();
	/// Register object factory.
	static void RegisterObject();
	/// Get box2d body.
	b2Body* GetBody() { return _body; }

	/// Set body type.
	void SetBodyType(BodyType2D::Type type);
	/// Set mass.
	void SetMass(float mass);
	/// Set inertia.
	void SetInertia(float inertia);
	/// Set mass center.
	void SetMassCenter(const Vector2F& center);
	/// Return whether to calculate mass and inertia from collision shapes automatically.
	bool GetUseFixtureMass() const { return _useFixtureMass; }
	/// Apply world transform from the Box2D body. Called by PhysicsWorld2D.
	void ApplyWorldTransform();
	/// Apply specified world position & rotation. Called by PhysicsWorld2D.
	void ApplyWorldTransform(const Vector3F& newWorldPosition,const Quaternion& newWorldRotation);

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack()override;
private:
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();
	/// Remove the rigid body from the physics world.
	void RemoveBodyFromWorld();

	/// Physics world 2d.
	WeakPtr<PhysicsWorld2D> _physicsWorld2d;
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