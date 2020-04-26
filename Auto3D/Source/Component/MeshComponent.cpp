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

void AMeshComponent::SetMesh(OMesh* mesh)
{
	if (mesh)
	{
		_mesh = mesh;
		SetGeometry(mesh->CreateGeometry());
	}
}

void AMeshComponent::SetMeshAttr(FResourceRef model)
{
	OMesh* mesh = new OMesh;
	mesh->Load(model._pathName.CString());
	SetMesh(mesh);
}

FResourceRef AMeshComponent::GetMeshAttr() const
{
	return FResourceRef(OMesh::GetTypeNameStatic(), ResourceName(_mesh));
}

}