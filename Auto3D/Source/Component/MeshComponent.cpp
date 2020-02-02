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

void AMeshComponent::SetMesh(SPtr<FMesh>& mesh)
{
	if (mesh)
	{
		_mesh = mesh;
	}
}

}