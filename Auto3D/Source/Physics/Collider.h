#pragma once
#include "../Scene/Node.h"

#include <LinearMath/btMotionState.h>

//#include "RigidBody.h"

class btCompoundShape;
class btCollisionShape;
class btRigidBody;
class btTransform;

namespace Auto3D 
{

/// Collision shape type.
namespace ShapeType
{
	enum Type
	{
		BOX = 0,
		SPHERE,
		STATICPLANE,
		CYLINDER,
		CAPSULE,
		CONE,
		TRIANGLEMESH,
		CONVEXHULL,
		TERRAIN,
		GIMPACTMESH
	};
};


class PhysicsWorld;

struct RigidBody : public RefCounted, public btMotionState
{
	RigidBody(PhysicsWorld* physicsWorld):
		_physicsWorld(physicsWorld)
	{}

	~RigidBody()
	{
		SafeDelete(_body);
	}
	/// Return initial world transform to Bullet.
	void getWorldTransform(btTransform& worldTrans) const override;
	/// Update world transform from Bullet.
	void setWorldTransform(const btTransform& worldTrans) override;

	void CreateRigidBody(float mass, const Vector3F& inertia, btCollisionShape* shape, const Vector3F& postion,const Quaternion& quaternion);

	bool _isDynamic;
	float _mass;
	Vector3F _inertia;
	Vector3F _postion;
	Quaternion _quaternion;
	/// Bullet rigid body.
	btRigidBody* _body;

	SharedPtr<PhysicsWorld> _physicsWorld;
};

class Collider : public Node
{
	REGISTER_OBJECT_CLASS(Collider, Node)
public:
	/// Construct
	Collider();
	/// Destructor
	virtual ~Collider();
	/// Register object factory.
	static void RegisterObject();
	
	/// Set physics world position
	void SetPhysicsWroldPostion(const Vector3F& pos);
	/// Get shape
	btCollisionShape* GetShape() { return _shape; }
	/// Create rigidBody.
	void SetRigidBody(float mass, const Vector3F& inertia);

	/// This function is called when the parent node of this class is assigned.
	virtual void ParentCallBack() override;

protected:
	/// Physics world form this collider
	SharedPtr<PhysicsWorld> _physicsWorld;
	/// Rigidbody form this collider
	SharedPtr<RigidBody> _rigidBody;
	/// Cached world scale.
	Vector3F _cachedWorldScale;
	/// Shape form this collider
	btCollisionShape* _shape;
	/// Shape type
	ShapeType::Type _shapeType;
};

}