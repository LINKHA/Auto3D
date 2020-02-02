#include "Component/MeshComponent.h"

namespace Auto3D
{

void AMeshComponent::BeginPlay()
{
	Super::BeginPlay();
}
void AMeshComponent::TickComponent(float deltaTime)
{
	Super::BeginPlay();
}

void AMeshComponent::SetMesh(SPtr<OMesh>& mesh)
{
	if (mesh)
	{
		_mesh = mesh;
	}
}

void AMeshComponent::SetMeshAttr(FResourceRef model)
{
	SPtr<OMesh> mesh = MakeShared<OMesh>();
	mesh->Load(model._name.CString());
	SetMesh(mesh);
}

FResourceRef AMeshComponent::GetMeshAttr() const
{
	return FResourceRef(OMesh::GetTypeNameStatic(), FString()/*ResourceName(_mesh)*/);
}

}