#pragma once
#include "Component/ActorComponent.h"
#include "Adapter/Ptr.h"

#include <LinearMath/btMotionState.h>

class btRigidBody;
class btCompoundShape;

namespace Auto3D 
{
class FPhysicsWorld;

class AUTO_API ARigidBody : public AActorComponent, public btMotionState
{
	DECLARE_A_CLASS(ARigidBody, AActorComponent)
public:
	/// Construct
	ARigidBody();
	/// Destructor
	~ARigidBody();

	/// Return initial world transform to Bullet.
	void getWorldTransform(btTransform& worldTrans) const override;
	/// Update world transform from Bullet.
	void setWorldTransform(const btTransform& worldTrans) override;

	/// Update mass and inertia to the Bullet rigid body. Readd body to world if necessary: if was in world and the Bullet collision shape to use changed.
	void UpdateMass();
	/// Set mass
	void SetMass(float mass);
	/// Get mass
	float GetMass() { return _mass; }
	/// Return Bullet rigid body.
	btRigidBody* GetBody() const { return _body; }
	/// Return compound shape.
	btCompoundShape* GetCompoundShape() { return _compoundShape; }
	/// Remove the rigid body.
	void ReleaseBody();

	/// This function is called when the parent node of this class is assigned.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent) override;
private:
	/// Create the rigid body, or re-add to the physics world with changed flags. Calls UpdateMass().
	void AddBodyToWorld();
	/// Remove the rigid body from the physics world.
	void RemoveBodyFromWorld();

	/// bullet rigid body (Automatically destructs when the physical world is eliminated).
	btRigidBody* _body;
	/// Bullet compound collision shape.
	btCompoundShape* _compoundShape;
	/// Rigidbody mass
	float _mass;
	/// Is dynamic ot staitc
	bool _isDynamic;
	/// First frame didn't calculate physics
	bool _isDirty;
	/// Body exists in world flag.
	bool _inWorld;
};

}