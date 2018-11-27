#include "RenderComponent.h"
#include "Renderer.h"
#include "DebugNew.h"

namespace Auto3D {
RenderComponent::RenderComponent(Ambient* ambient)
	:Super(ambient)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::RegisterShadow(RenderComponent * com)
{
	GetSubSystem<Renderer>()->AddShadowMap(com);
}

void RenderComponent::RegisterOpaque(RenderComponent * com)
{
	GetSubSystem<Renderer>()->AddOpaqueGeometry(com);
}

void RenderComponent::RegisterCustom(RenderComponent * com)
{
	GetSubSystem<Renderer>()->AddCustomGeometry(com);
}
void RenderComponent::RegisterTranslucent(RenderComponent * com)
{
	GetSubSystem<Renderer>()->AddTranslucentGeometry(com);
}
void RenderComponent::UnloadShadow(RenderComponent* com)
{
	GetSubSystem<Renderer>()->RemoveShadowMap(com);
}
void RenderComponent::UnloadOpaque(RenderComponent* com)
{
	GetSubSystem<Renderer>()->RemoveOpaqueGeometry(com);
}
void RenderComponent::UnloadCustom(RenderComponent* com)
{
	GetSubSystem<Renderer>()->RemoveCustomGeometry(com);
}
void RenderComponent::UnloadTranslucent(RenderComponent* com)
{
	GetSubSystem<Renderer>()->RemoveTranslucentGeometry(com);
}

}