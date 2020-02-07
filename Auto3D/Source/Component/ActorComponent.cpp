#include "Component/ActorComponent.h"
#include "Gameplay/World.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"

namespace Auto3D
{

AActorComponent::AActorComponent() :
	_hasBegunPlay(false),
	_ownerPrivate(nullptr),
	_worldPrivate(nullptr)
{
}

void AActorComponent::BeginPlay()
{
	_hasBegunPlay = true;
}

void AActorComponent::TickComponent(float deltaTime)
{

}

AActor* AActorComponent::GetOwner() const
{
	return _ownerPrivate;
}

AWorld* AActorComponent::GetWorld() const
{
	return _worldPrivate;
}

void AActorComponent::OnActorSet(AActor* newParent, AActor* oldParent)
{
	_ownerPrivate = newParent;

	if (!newParent)
	{
		return;
	}

	if (newParent->GetWorld())
		_worldPrivate = newParent->GetWorld();
}

}