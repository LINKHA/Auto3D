#pragma once
#include "Component/ActorComponent.h"

namespace Auto3D
{

class AUTO_API ASceneComponent : public AActorComponent
{
	DECLARE_CLASS(ASceneComponent, AActorComponent)
public:
	ASceneComponent() {}
	virtual ~ASceneComponent() {}

	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

};

}