#pragma once
#include "Component/LightComponent.h"

namespace Auto3D
{

class AUTO_API ASpotLightComponent : public ALightComponent
{
	DECLARE_A_CLASS(ASpotLightComponent, ALightComponent)
public:
	ASpotLightComponent();
	~ASpotLightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);
};

}