#pragma once
#include "Component/LightComponent.h"

namespace Auto3D
{

class AUTO_API APointLightComponent : public ALightComponent
{
	DECLARE_A_CLASS(APointLightComponent, ALightComponent)
public:
	APointLightComponent();
	virtual ~APointLightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

};

}