#pragma once
#include "Core/GameManager.h"
namespace Auto3D {

class APhysicsWorld;
class PhysicsWorld2D;

/// APhysics sub system 
class AUTO_API APhysics : public ABaseModule
{
	REGISTER_OBJECT_CLASS(APhysics, ABaseModule)
public:
	/// Construct
	APhysics();
	/// Destructor
	~APhysics();
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
	void AddPhysicsWorld2D(PhysicsWorld2D* physicsWorld2d);
	/// Remove physics world.
	void RemovePhysicsWorld2D(PhysicsWorld2D* physicsWorld2d);
	/// Set active physics world. 
	void SetActivePhysicsWrold2D(PhysicsWorld2D* physicsWorld2d);
	/// Get active 2d physics world.
	PhysicsWorld2D* GetActivePhysicsWorld2D()const { return _activeWorld2d; }

private:
	/// All scene physics world.
	TVector<APhysicsWorld*> _physicsWorlds;
	/// Active physics world.
	APhysicsWorld* _activeWorld;
	/// All scene2d physics world.
	TVector<PhysicsWorld2D*> _physicsWorld2ds;
	/// Active 2D physics world.
	PhysicsWorld2D* _activeWorld2d;

};

/// Register physics related object factories and attributes.
AUTO_API void RegisterPhysicsLibrary();

}