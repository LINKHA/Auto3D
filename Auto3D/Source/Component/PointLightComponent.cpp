#include "Component/PointLightComponent.h"

namespace Auto3D
{

APointLightComponent::APointLightComponent()
{
	_lightType = ELightType::PointLight;
	Reset();
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

void APointLightComponent::Reset()
{
	m_position = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_position_viewSpace[0] = 0.0f;
	m_position_viewSpace[1] = 0.0f;
	m_position_viewSpace[2] = 0.0f;
	m_position_viewSpace[3] = 0.0f;
	m_ambientPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_diffusePower = { 1.0f, 1.0f, 1.0f, 850.0f };
	m_specularPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_spotDirectionInner = { 0.0f,-0.4f,-0.6f, 0.0f };
	m_spotDirectionInner_viewSpace[0] = 0.0f;
	m_spotDirectionInner_viewSpace[1] = 0.0f;
	m_spotDirectionInner_viewSpace[2] = 0.0f;
	m_spotDirectionInner_viewSpace[3] = 0.0f;
	m_attenuationSpotOuter = { 1.0f, 0.0f, 1.0f, 91.0f };
}

}