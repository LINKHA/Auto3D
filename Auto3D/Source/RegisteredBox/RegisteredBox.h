#pragma once

#include "Container/Ptr.h"
#include "Container/Vector.h"

namespace Auto3D
{

class AWorld;
class ACamera;
class AScene2D;
class ACamera2D;

/// Manages messaging between the user and the engine
class AUTO_API FRegisteredBoxModule : public FRefCounted
{
public:
	/// Construct.
	FRegisteredBoxModule();
	/// Destructor
	~FRegisteredBoxModule();
	/// Register world
	void RegisterWorld(AWorld* world) { SetActiveWorld(world);  _worlds.Push(world); }
	/// Register scene2d
	void RegisterScene2D(AScene2D* scene2d) { SetActiveScene2D(scene2d); _scene2ds.Push(scene2d); }
	/// Set active world.
	void SetActiveWorld(AWorld* world);
	/// Set active scene2d.
	void SetActiveScene2D(AScene2D* scene2d);

	/// Return worlds.
	const TVector<AWorld*>& GetWorlds() { return _worlds; }
	/// Return scene2d.
	const TVector<AScene2D*>& GetScene2Ds() { return _scene2ds; }
	/// Get active world.
	AWorld* GetActiveWorld();
	/// Get active scene2d.
	AScene2D* GetActiveScene2D();
private:
	/// AWorld array.
	TVector<AWorld*> _worlds;
	/// AScene2D array. 
	TVector<AScene2D*> _scene2ds;

	/// Active world.
	AWorld* _activeWorld;
	/// Active scene2d.
	AScene2D* _activeScene2d;
};

}