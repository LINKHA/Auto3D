#include "Controller.h"
#include "GameFramework/Actor.h"

namespace Auto3D
{
REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(AController)
	.constructor<>()
	;
}

AController::AController():
	_isAttachToActor(false),
	_attachedActor(nullptr)
{
}

AController::~AController()
{

}

void AController::BeginPlay()
{
	Super::BeginPlay();
	AttachToActor(GetOwner());
}

void AController::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

AActor* AController::GetAttachedActor()
{
	if (!IsAttachToActor() || !_attachedActor)
		ErrorString("Fail get attached actor.");

	return _attachedActor;
}

void AController::AttachToActor(AActor* actor)
{
	if (!actor || _attachedActor == GetOwner())
		return;

	UnAttachToActor();

	if(actor!=GetOwner())
		actor->AddComponent(this);

	_attachedActor = actor;
	_isAttachToActor = true;
}

void AController::UnAttachToActor()
{
	if (_isAttachToActor)
	{
		if (GetOwner())
		{
			AActor* owner = GetOwner();
			owner->RemoveComponent(this);
		}
	}

	_attachedActor = nullptr;
	_isAttachToActor = false;
}

}