#include "Component/SceneComponent.h"
#include "Gameplay/Actor.h"

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


}


