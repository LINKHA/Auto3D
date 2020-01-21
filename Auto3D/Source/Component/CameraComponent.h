#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"

namespace Auto3D
{

class AUTO_API ACameraComponent : public AActorComponent
{
	DECLARE_CLASS(ACameraComponent, AActorComponent)
public:
	ACameraComponent();
	~ACameraComponent();
	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

};

}