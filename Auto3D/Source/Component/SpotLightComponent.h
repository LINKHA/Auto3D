#pragma once
#include "Component/PointLightComponent.h"

namespace Auto3D
{

class AUTO_API ASpotLightComponent : public APointLightComponent
{
	DECLARE_A_CLASS(ASpotLightComponent, ALightComponent)
public:
	ASpotLightComponent();
	virtual ~ASpotLightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);
};

}