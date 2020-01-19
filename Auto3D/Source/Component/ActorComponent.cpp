#include "Component/ActorComponent.h"
#include "Gameplay/World.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"

namespace Auto3D
{

AActorComponent::AActorComponent()
{
}

bool AActorComponent::AttachToActor(SPtr<AActor> owner)
{
	if (!owner)
	{
		ErrorString("Fail attach to actor.");
		return false;
	}

	_ownerPrivate = owner;
	/*if (owner->GetWorld())
		_worldPrivate = owner->GetWorld();*/
	
	return true;
}

SPtr<AActor> AActorComponent::GetOwner() const
{
	return _ownerPrivate.lock();
}

SPtr<AWorld> AActorComponent::GetWorld() const
{
	return _worldPrivate.lock();
}

}