#include "Component/CameraComponent.h"
#include "Math/Matrix4x4.h"
#include "Component/Transform.h"
#include "Gameplay/Actor.h"

namespace Auto3D
{


ACameraComponent::ACameraComponent()
{
}

ACameraComponent::~ACameraComponent()
{
}

void ACameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

TMatrix3x4F ACameraComponent::EffectiveWorldTransform() const
{
	AActor* owner = GetOwner();
	ATransform* transform = owner->GetTransform();
	TMatrix3x4F worldTransform(transform->GetWorldPosition(), transform->GetWorldRotation(), 1.0f);

	return worldTransform;
}


const TMatrix3x4F& ACameraComponent::GetViewMatrix()
{
	AActor* owner = GetOwner();
	ATransform* transform = owner->GetTransform();

	if(transform->IsDirty())
		_viewMatrix = EffectiveWorldTransform().Inverse();

	return _viewMatrix;
}

}