#pragma once
#include "Core/Object.h"
#include "Adapter/Ptr.h"

namespace Auto3D
{
class AActor;
class AWorld;
class AActorComponent : public OObject
{
	DECLARE_A_CLASS(AActorComponent, OObject)
public:
	AActorComponent();
	virtual ~AActorComponent(){}
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	/// Follow the Outer chain to get the  AActor  that 'Owns' this component.
	virtual AActor* GetOwner() const;
	/// Getter for the cached world pointer, will return null if the component is not actually spawned in a level.
	virtual AWorld* GetWorld() const;

	/// Indicates that BeginPlay has been called, but EndPlay has not yet.
	bool HasBegunPlay() const { return _hasBegunPlay; }

	/// This is the old name of the tick function. We just want to avoid mistakes with an attempt to override this.
	virtual void Tick(float deltaTime) final { assert(0); }

public:
	/// Handle being assigned to a new parent node.
	virtual void OnActorSet(AActor* newParent, AActor* oldParent);

private:
	/// Cached pointer to owning actor.
	mutable AActor* _ownerPrivate;

	/**
	 * Pointer to the world that this component is currently registered with.
	 * This is only non-NULL when the component is registered.
	 */
	AWorld* _worldPrivate;
	/// Indicates that BeginPlay has been called, but EndPlay has not yet.
	bool _hasBegunPlay;
};

}