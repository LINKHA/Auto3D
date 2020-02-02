#pragma once
#include "Component/ActorComponent.h"
#include "Resource/ResourceRef.h"

namespace Auto3D
{

class AUTO_API ASceneComponent : public AActorComponent
{
	DECLARE_A_CLASS(ASceneComponent, AActorComponent)
public:
	ASceneComponent() {}
	virtual ~ASceneComponent() {}

	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

};

}