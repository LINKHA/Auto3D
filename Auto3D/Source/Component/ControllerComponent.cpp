#include "Component/ControllerComponent.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"
#include "Component/ControllerManager.h"
namespace Auto3D
{

AControllerComponent::AControllerComponent() :
	_isAttachToActor(false),
	_attachedActor(nullptr)
{
	
}

AControllerComponent::~AControllerComponent()
{

}

void AControllerComponent::BeginPlay()
{
	Super::BeginPlay();
	AttachToActor(GetOwner());
	GControllerManager::Get().RegisterController(this);
}

void AControllerComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

AActor* AControllerComponent::GetAttachedActor()
{
	if (!IsAttachToActor() || !_attachedActor)
		ErrorString("Fail get attached actor.");

	return _attachedActor;
}

void AControllerComponent::AttachToActor(AActor* actor)
{
	if (!actor || _attachedActor == GetOwner())
		return;

	UnAttachToActor();

	if (actor != GetOwner())
		actor->AddComponent(this);

	_attachedActor = actor;
	_isAttachToActor = true;
}

void AControllerComponent::UnAttachToActor()
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