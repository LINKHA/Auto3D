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
	void RegisterWorld(SPtr<AWorld> world) { SetActiveWorld(world);  _worlds.Push(world); }
	/// Set active world.
	void SetActiveWorld(SPtr<AWorld> world);

	/// Return worlds.
	const TVector<SPtr<AWorld>>& GetWorlds() { return _worlds; }
	/// Get active world.
	SPtr<AWorld> GetActiveWorld();
private:
	/// AWorld array.
	TVector<SPtr<AWorld>> _worlds;

	/// Active world.
	SPtr<AWorld> _activeWorld;
};

}