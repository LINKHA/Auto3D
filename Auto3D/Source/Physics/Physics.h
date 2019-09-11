#pragma once
#include "../Base/GameManager.h"

namespace Auto3D {

class PhysicsWorld;

/// Physics sub system 
class AUTO_API Physics : public BaseModule
{
	REGISTER_OBJECT_CLASS(Physics, BaseModule)
public:
	/// Construct
	Physics();
	/// Destructor
	~Physics();
	/// Update active physics world.
	void Update();
	/// Add physics world.
	void AddPhysicsWorld(PhysicsWorld* activeWorlds);
	/// Remove physics world.
	void RemovePhysicsWorld(PhysicsWorld* activeWorlds);
	/// Set active physics world. 
	void SetActivePhysicsWrold(PhysicsWorld* activeWorlds);
private:
	/// All scene physics world.
	Vector<PhysicsWorld*> _physicsWorlds;
	/// Active physics world.
	PhysicsWorld* _activeWorlds;
};

/// Register physics related object factories and attributes.
AUTO_API void RegisterPhysicsLibrary();

}