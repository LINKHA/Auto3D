#include "Component/ActorComponent.h"
#include "Gameplay/World.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"

namespace Auto3D
{

AActorComponent::AActorComponent() :
	_hasBegunPlay(false)
{
}

void AActorComponent::BeginPlay()
{
	_hasBegunPlay = true;
}

void AActorComponent::TickComponent(float deltaTime)
{

}

bool AActorComponent::AttachToActor(AActor* owner)
{
	if (!owner)
	{
		ErrorString("Fail attach to actor.");
		return false;
	}

	_ownerPrivate = owner;
	if (owner->GetWorld())
		_worldPrivate = owner->GetWorld();
	
	return true;
}

AActor* AActorComponent::GetOwner() const
{
	return _ownerPrivate;
}

AWorld* AActorComponent::GetWorld() const
{
	return _worldPrivate;
}

}