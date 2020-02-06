#include "Component/SceneComponent.h"
#include "Gameplay/Actor.h"
#include "Resource/Material.h"
#include "Resource/Shader.h"


namespace Auto3D
{

void ASceneComponent::BeginPlay()
{
	Super::BeginPlay();
	AActor* owner = GetOwner();
	if (owner)
	{
		owner->SetFlag(NF_GEOMETRY, true);
	}
}
void ASceneComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

void ASceneComponent::SetMaterial(OMaterial* material)
{
	if (material)
	{
		_material = material;

		_pass.CreateShaderHash(material->GetShaderProgram().GetVertexShader()->GetPathName(),
			material->GetShaderProgram().GetPixelShader()->GetPathName());
	}
}

OMaterial* ASceneComponent::GetMaterial() const
{
	return _material;
}

void ASceneComponent::SetGeometryName(const FString& name)
{
	_pass.CreateGeometryHash(name);
}

const FPass& ASceneComponent::GetPass()
{
	return _pass;
}

}


