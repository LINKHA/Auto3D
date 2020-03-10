#include "Component/DirectionalLightComponent.h"

namespace Auto3D
{

ADirectionalLightComponent::ADirectionalLightComponent()
{
	_lightType = ELightType::DirectionalLight;
}

ADirectionalLightComponent::~ADirectionalLightComponent()
{

}


void ADirectionalLightComponent::BeginPlay()
{
	Super::BeginPlay();
}


void ADirectionalLightComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}


}