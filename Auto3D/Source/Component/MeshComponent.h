#pragma once
#include "Component/ActorComponent.h"

namespace Auto3D
{

class AUTO_API AMeshComponent : public AActorComponent
{
	DECLARE_CLASS(AMeshComponent, AActorComponent)
public:
	AMeshComponent() {}
	~AMeshComponent() {}
	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;
};
}