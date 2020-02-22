#pragma once
#include "Container/Ptr.h"
#include "Container/Vector.h"
#include "Container/Singleton.h"

namespace Auto3D {

class FPhysicsWorld;

/// APhysics sub system 
class AUTO_API GPhysicsModule
{
	REGISTER_SINGLETON(GPhysicsModule)
public:
	/// Construct
	GPhysicsModule();
	/// Destructor
	~GPhysicsModule();
	/// Update active physics world.
	void Update();
	/// Add physics world.
	void AddPhysicsWorld(FPhysicsWorld* physicsWorld);
	/// Remove physics world.
	void RemovePhysicsWorld(FPhysicsWorld* physicsWorld);
	/// Set active physics world. 
	void SetActivePhysicsWorld(FPhysicsWorld* physicsWorld);
	/// Get active physics world.
	FPhysicsWorld* GetActivePhysicsWorld()const { return _activeWorld; }
private:
	/// All scene physics world.
	TVector<FPhysicsWorld*> _physicsWorlds;
	/// Active physics world.
	FPhysicsWorld* _activeWorld;

};

}