#include "Component/CameraComponent.h"
#include "Math/Matrix4x4.h"
#include "Component/Transform.h"
#include "Gameplay/Actor.h"

#include <bgfx/bgfx.h>
#include <bx/math.h>

namespace Auto3D
{

static const float DEFAULT_NEARCLIP = 0.1f;
static const float DEFAULT_FARCLIP = 100.0f;
static const float DEFAULT_FOV = 60.0f;

ACameraComponent::ACameraComponent():
	_viewMatrix(TMatrix3x4F::IDENTITY),
	_nearClip(DEFAULT_NEARCLIP),
	_farClip(DEFAULT_FARCLIP),
	_fov(DEFAULT_FOV),
	_aspectRatio(1.0f)
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

void ACameraComponent::SetNearClip(float nearClip)
{
	_nearClip = Max(nearClip, M_EPSILON);
}

void ACameraComponent::SetFarClip(float farClip)
{
	_farClip = Max(farClip, M_EPSILON);
}

void ACameraComponent::SetFov(float fov)
{
	_fov = Clamp(fov, 0.0f, 180.0f);
}

void ACameraComponent::SetAspectRatio(float aspectRatio)
{
	_aspectRatio = Max(aspectRatio, M_EPSILON);
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

TMatrix4x4F ACameraComponent::GetProjectionMatrix(bool apiSpecific) const
{
	float proj[16];
	bx::mtxProj(proj, _fov,_aspectRatio, _nearClip, _farClip, bgfx::getCaps()->homogeneousDepth);

	TMatrix4x4F matrix(proj);
	return matrix;
}

}