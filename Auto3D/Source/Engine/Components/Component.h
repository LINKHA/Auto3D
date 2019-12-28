#pragma once
#include "Scene/Transform.h"

namespace Auto3D
{
class AActor;
class AWorld;

class AUTO_API AComponent : public ATransform
{
	DECLARE_CLASS(AComponent, ATransform)
public:
	AComponent();
	~AComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);
	/// Follow the Outer chain to get the  AActor  that 'Owns' this component.
	AActor* GetOwner()const;
	/// Handle being assigned to a new actor node.
	void OnActorSet(AActor* actor);
	/** Indicates that BeginPlay has been called, but EndPlay has not yet */
	bool HasBegunPlay() const { return _hasBegunPlay; }

	/// This is the old name of the tick function. We just want to avoid mistakes with an attempt to override this.
	virtual void Tick(float deltaTime) final { assert(0); }
private:
	/// Owner with in onactor set.
	mutable AActor* _owner;
	/// Indicates that BeginPlay has been called, but EndPlay has not yet.
	bool _hasBegunPlay;
};

}