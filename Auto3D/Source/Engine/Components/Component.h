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
	AComponent() {}

	~AComponent() {}

	void BeginPlay();

	// Called every frame
	void TickComponent(float DeltaTime);

	AActor* GetOwner()const;


	/// Handle being assigned to a new actor node.
	void OnActorSet(AActor* actor);
private:
	mutable AActor* _owner;
};

}