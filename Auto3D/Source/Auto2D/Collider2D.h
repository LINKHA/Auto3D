#pragma once
#include "Node2D.h"
namespace Auto3D{
	
class RigidBody2D;

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
private:
	/// Rigid body.
	WeakPtr<RigidBody2D> _rigidBody;
};

}

