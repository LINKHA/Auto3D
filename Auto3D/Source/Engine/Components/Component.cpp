#include "Component.h"
#include "GameFramework/Actor.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AComponent)
	.constructor<>()
	;
}

AActor* AComponent::GetOwner()const
{
	return _owner;
}

void AComponent::OnActorSet(AActor* actor)
{
	if(actor)
		_owner = actor;
}

}