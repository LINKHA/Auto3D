#pragma once
#include "Component/LightComponent.h"

namespace Auto3D
{

class AUTO_API ADirectionalLightComponent : public ALightComponent
{
	DECLARE_A_CLASS(ADirectionalLightComponent,ALightComponent)
public:
	ADirectionalLightComponent();
	~ADirectionalLightComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);
};

}