#include "RenderComponent.h"
#include "Renderer.h"
#include "NewDef.h"

namespace Auto3D {
RenderComponent::RenderComponent(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::RegisterShadow(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->AddShadowMap(com);
}

void RenderComponent::RegisterOpaque(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->AddOpaqueGeometry(com);
}

void RenderComponent::RegisterCustom(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->AddCustomGeometry(com);
}
void RenderComponent::RegisterTranslucent(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->AddTranslucentGeometry(com);
}
void RenderComponent::UnloadShadow(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->RemoveShadowMap(com);
}
void RenderComponent::UnloadOpaque(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->RemoveOpaqueGeometry(com);
}
void RenderComponent::UnloadCustom(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->RemoveCustomGeometry(com);
}
void RenderComponent::UnloadTranslucent(SharedPtr<RenderComponent> com)
{
	GetSubSystem<Renderer>()->RemoveTranslucentGeometry(com);
}

}