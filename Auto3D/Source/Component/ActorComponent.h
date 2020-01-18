#include "Core/Object.h"

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
	bool AttachToActor(AActor* owner);
	/// Follow the Outer chain to get the  AActor  that 'Owns' this component.
	virtual AActor* GetOwner() const;
	/// Getter for the cached world pointer, will return null if the component is not actually spawned in a level.
	virtual AWorld* GetWorld() const;
private:
	/// Cached pointer to owning actor.
	mutable AActor* _ownerPrivate;

	/**
	 * Pointer to the world that this component is currently registered with.
	 * This is only non-NULL when the component is registered.
	 */
	AWorld* _worldPrivate;
};

}