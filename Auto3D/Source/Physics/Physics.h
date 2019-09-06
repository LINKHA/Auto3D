#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

class PhysicsWorld;

/// Physics sub system 
class AUTO_API Physics : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(Physics, BaseSubsystem)
public:
	/// Construct
	Physics();
	/// Destructor
	~Physics();

	void Update();

	void AddPhysicsWorld(PhysicsWorld* activeWorlds);

	void RemovePhysicsWorld(PhysicsWorld* activeWorlds);

	void SetActivePhysicsWrold(PhysicsWorld* activeWorlds);
private:
	Vector<PhysicsWorld*> _physicsWorlds;

	PhysicsWorld* _activeWorlds;
};

/// Register physics related object factories and attributes.
AUTO_API void RegisterPhysicsLibrary();

}