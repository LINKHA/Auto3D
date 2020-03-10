#include "Component/SpotLightComponent.h"

namespace Auto3D
{

ASpotLightComponent::ASpotLightComponent()
{
	_lightType = ELightType::SpotLight;
	Reset();
}
ASpotLightComponent:: ~ASpotLightComponent()
{

}

void ASpotLightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ASpotLightComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

}