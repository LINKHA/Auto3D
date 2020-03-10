#include "Component/PointLightComponent.h"

namespace Auto3D
{

APointLightComponent::APointLightComponent()
{
	_lightType = ELightType::PointLight;
}
APointLightComponent:: ~APointLightComponent()
{

}

void APointLightComponent::BeginPlay()
{
	Super::BeginPlay();
}

void APointLightComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

}