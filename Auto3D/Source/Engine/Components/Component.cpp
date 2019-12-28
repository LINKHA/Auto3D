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

AComponent::AComponent():
	_hasBegunPlay(false)
{}

AComponent::~AComponent()
{}

void AComponent::BeginPlay()
{
	Check(!_hasBegunPlay);
	_hasBegunPlay = true;
}

void AComponent::TickComponent(float deltaTime)
{

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