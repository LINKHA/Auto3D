#include "Component/GeometryComponent.h"
#include "Gameplay/Actor.h"
#include "Resource/Material.h"
#include "Resource/Shader.h"


namespace Auto3D
{

void AGeometryComponent::BeginPlay()
{
	Super::BeginPlay();
}
void AGeometryComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

void AGeometryComponent::SetMaterial(OMaterial* material)
{
	if (material)
	{
		_material = material;
		
		_pass.CreateShaderHash(material->GetShaderProgram().GetVertexShader()->GetPathName(),
			material->GetShaderProgram().GetPixelShader()->GetPathName());
	}
}

OMaterial* AGeometryComponent::GetMaterial() const
{
	return _material;
}

void AGeometryComponent::SetGeometryName(const FString& name)
{
	_pass.CreateGeometryHash(name);
}

const FPass& AGeometryComponent::GetPass()
{
	return _pass;
}

void AGeometryComponent::OnActorSet(AActor* newParent, AActor* oldParent)
{
	Super::OnActorSet(newParent, oldParent);

	if (oldParent)
	{
		TVector<AGeometryComponent*>& geometryComponents = oldParent->GetGeometryComponents();
		auto it = geometryComponents.Find(this);
		if (it != geometryComponents.End())
		{
			geometryComponents.Erase(it);
		}
		if (!geometryComponents.Size())
		{
			oldParent->SetFlag(NF_GEOMETRY, false);
		}
	}

	if (newParent)
	{
		newParent->GetGeometryComponents().Push(this);
		newParent->SetFlag(NF_GEOMETRY, true);
	}
}

}


