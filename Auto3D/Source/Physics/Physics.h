#pragma once
#include "Container/Ptr.h"
#include "Container/Vector.h"

namespace Auto3D {

class APhysicsWorld;
class APhysicsWorld2D;

/// APhysics sub system 
class AUTO_API FPhysicsModule : public FRefCounted
{
public:
	/// Construct
	FPhysicsModule();
	/// Destructor
	~FPhysicsModule();
	/// Update active physics world.
	void Update();
	/// Add physics world.
	void AddPhysicsWorld(APhysicsWorld* physicsWorld);
	/// Remove physics world.
	void RemovePhysicsWorld(APhysicsWorld* physicsWorld);
	/// Set active physics world. 
	void SetActivePhysicsWorld(APhysicsWorld* physicsWorld);
	/// Get active physics world.
	APhysicsWorld* GetActivePhysicsWorld()const { return _activeWorld; }
	/// Add physics world.
	void AddPhysicsWorld2D(APhysicsWorld2D* physicsWorld2d);
	/// Remove physics world.
	void RemovePhysicsWorld2D(APhysicsWorld2D* physicsWorld2d);
	/// Set active physics world. 
	void SetActivePhysicsWrold2D(APhysicsWorld2D* physicsWorld2d);
	/// Get active 2d physics world.
	APhysicsWorld2D* GetActivePhysicsWorld2D()const { return _activeWorld2d; }

private:
	/// All scene physics world.
	TVector<APhysicsWorld*> _physicsWorlds;
	/// Active physics world.
	APhysicsWorld* _activeWorld;
	/// All scene2d physics world.
	TVector<APhysicsWorld2D*> _physicsWorld2ds;
	/// Active 2D physics world.
	APhysicsWorld2D* _activeWorld2d;

};

}