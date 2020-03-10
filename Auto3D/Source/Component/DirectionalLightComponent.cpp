#include "Component/DirectionalLightComponent.h"

namespace Auto3D
{

ADirectionalLightComponent::ADirectionalLightComponent()
{
	_lightType = ELightType::DirectionalLight;
	Reset();
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

void ADirectionalLightComponent::Reset()
{
	m_position = { 0.5f,-1.0f, 0.1f, 0.0f };
	m_position_viewSpace[0] = 0.0f;
	m_position_viewSpace[1] = 0.0f;
	m_position_viewSpace[2] = 0.0f;
	m_position_viewSpace[3] = 0.0f;
	m_ambientPower = { 1.0f, 1.0f, 1.0f, 0.02f };
	m_diffusePower = { 1.0f, 1.0f, 1.0f, 0.4f };
	m_specularPower = { 1.0f, 1.0f, 1.0f, 0.0f };
	m_spotDirectionInner = { 0.0f, 0.0f, 0.0f, 1.0f };
	m_spotDirectionInner_viewSpace[0] = 0.0f;
	m_spotDirectionInner_viewSpace[1] = 0.0f;
	m_spotDirectionInner_viewSpace[2] = 0.0f;
	m_spotDirectionInner_viewSpace[3] = 0.0f;
	m_attenuationSpotOuter = { 0.0f, 0.0f, 0.0f, 1.0f };
}

}