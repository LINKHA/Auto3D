#include "Component/GeometryComponent.h"
#include "Gameplay/Actor.h"
#include "Resource/Material.h"
#include "Resource/Shader.h"
#include "Debug/Log.h"
#include "Component/TransformComponent.h"

namespace Auto3D
{

AGeometryComponent::AGeometryComponent()
{
	SetMaterial(nullptr);
}

AGeometryComponent::~AGeometryComponent()
{

}

void AGeometryComponent::BeginPlay()
{
	Super::BeginPlay();
	_pass._worldMatrix = &GetOwner()->GetTransform()->GetWorldTransform();
}
void AGeometryComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
	_pass._worldMatrix = &GetOwner()->GetTransform()->GetWorldTransform();
}

FPass& AGeometryComponent::GetPass()
{
	return _pass;
}

void AGeometryComponent::SetGeometry(FGeometry* geometry)
{
	if (!geometry)
	{
		ErrorString("Can not assign null geometry");
		return;
	}

	_pass._geometry = geometry;
}

void AGeometryComponent::SetMaterial(OMaterial* material)
{
	if (!material)
		material = OMaterial::DefaultMaterial();

	_pass._material = material;
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


