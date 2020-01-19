#pragma once
#include "Core/Object.h"
#include "Adapter/Ptr.h"

namespace Auto3D
{
class AActor;
class AWorld;
class AActorComponent : public AObject
{
public:
	AActorComponent();
	virtual ~AActorComponent(){}
	/// Attach to actor,and set owner private and world private.
	bool AttachToActor(SPtr<AActor> owner);
	/// Follow the Outer chain to get the  AActor  that 'Owns' this component.
	virtual SPtr<AActor> GetOwner() const;

	/// Getter for the cached world pointer, will return null if the component is not actually spawned in a level.
	virtual SPtr<AWorld> GetWorld() const;
private:
	/// Cached pointer to owning actor.
	mutable WPtr<AActor> _ownerPrivate;

	/**
	 * Pointer to the world that this component is currently registered with.
	 * This is only non-NULL when the component is registered.
	 */
	WPtr<AWorld> _worldPrivate;
};

}