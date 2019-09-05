#pragma once
#include "../Scene/Node.h"

#include <LinearMath/btMotionState.h>

class btRigidBody;
class btCompoundShape;

namespace Auto3D 
{
class PhysicsWorld;

class RigidBody : public Node, public btMotionState
{
	REGISTER_OBJECT_CLASS(RigidBody, Node)
public:
	/// Construct
	RigidBody();
	/// Destructor
	~RigidBody();
	/// Register object factory.
	static void RegisterObject();
	/// Return initial world transform to Bullet.
	void getWorldTransform(btTransform& worldTrans) const override;
	/// Update world transform from Bullet.
	void setWorldTransform(const btTransform& worldTrans) override;

	/// Update mass and inertia to the Bullet rigid body. Readd body to world if necessary: if was in world and the Bullet collision shape to use changed.
	void UpdateMass();
	/// Set mass
	void SetMass(float mass) { _mass = mass; }
	/// Get mass
	float GetMass() { return _mass; }

	/// Return Bullet rigid body.
	btRigidBody* GetBody() const { return _body.Get(); }

	btCompoundShape* GetCompoundShape() { return _compoundShape.Get(); }

	virtual void ParentCallBack()override;

private:
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();

	/// Physics world form this rigidBody.
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// bullet rigid body
	UniquePtr<btRigidBody> _body;
	/// Bullet compound collision shape.
	UniquePtr<btCompoundShape> _compoundShape;
	/// Rigidbody mass
	float _mass;
	/// Is dynamic ot staitc
	bool _isDynamic;
	/// First frame didn't calculate physics
	bool _isDirty;

};

}