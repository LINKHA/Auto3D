#pragma once
#include "Core/GameManager.h"
namespace Auto3D {

class PhysicsWorld;
class PhysicsWorld2D;

/// Physics sub system 
class AUTO_API Physics : public ABaseModule
{
	REGISTER_OBJECT_CLASS(Physics, ABaseModule)
public:
	/// Construct
	Physics();
	/// Destructor
	~Physics();
	/// Update active physics world.
	void Update();
	/// Add physics world.
	void AddPhysicsWorld(PhysicsWorld* physicsWorld);
	/// Remove physics world.
	void RemovePhysicsWorld(PhysicsWorld* physicsWorld);
	/// Set active physics world. 
	void SetActivePhysicsWorld(PhysicsWorld* physicsWorld);
	/// Get active physics world.
	PhysicsWorld* GetActivePhysicsWorld()const { return _activeWorld; }
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
	TVector<PhysicsWorld*> _physicsWorlds;
	/// Active physics world.
	PhysicsWorld* _activeWorld;
	/// All scene2d physics world.
	TVector<PhysicsWorld2D*> _physicsWorld2ds;
	/// Active 2D physics world.
	PhysicsWorld2D* _activeWorld2d;

};

/// Register physics related object factories and attributes.
AUTO_API void RegisterPhysicsLibrary();

}