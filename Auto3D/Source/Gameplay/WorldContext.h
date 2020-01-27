#pragma once
#include "Container/Singleton.h"
#include "Container/Vector.h"
#include "Adapter/Ptr.h"
namespace Auto3D
{

class AWorld;
/// Manages messaging between the user and the engine
class AUTO_API FWorldContext
{
	REGISTER_SINGLETON(FWorldContext)
protected:
	/// Construct.
	FWorldContext();
	/// Destructor
	~FWorldContext();
public:
	/// Register world
	void RegisterWorld(AWorld* world) { SetActiveWorld(world);  _worlds.Push(world); }
	/// Set active world.
	void SetActiveWorld(AWorld* world);

	void DeleteWorld(AWorld* world);
	void DeleteAllWorld();

	/// Return worlds.
	const TVector<AWorld*>& GetWorlds() { return _worlds; }
	/// Get active world.
	AWorld* GetActiveWorld();

	AWorld* NewWorld();
private:
	/// AWorld array.
	TVector<AWorld*> _worlds;

	/// Active world.
	AWorld* _activeWorld;
};

}