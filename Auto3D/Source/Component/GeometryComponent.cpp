#include "Component/GeometryComponent.h"
#include "Gameplay/Actor.h"
#include "Resource/Material.h"
#include "Resource/Shader.h"
#include "Debug/Log.h"

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

void AGeometryComponent::_SetGeometry(size_t index, FGeometry* geometry)
{
	if (!geometry)
	{
		ErrorString("Can not assign null geometry");
		return;
	}

	if (index < _batches.Size())
		_batches[index]._geometry = geometry;
	else
		ErrorStringF("Out of bounds batch index %d for setting geometry", (int)index);
}

void AGeometryComponent::_SetMaterial(OMaterial* material)
{
	if (!material)
		material = OMaterial::DefaultMaterial();

	for (size_t i = 0; i < _batches.Size(); ++i)
		_batches[i]._material = material;
}

void AGeometryComponent::_SetMaterial(size_t index, OMaterial* material)
{
	if (index < _batches.Size())
	{
		if (!material)
			material = OMaterial::DefaultMaterial();
		_batches[index]._material = material;
	}
	else
		ErrorStringF("Out of bounds batch index %d for setting material", (int)index);
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


