#include "Component/CameraComponent.h"
#include "Math/Matrix4x4.h"
#include "Component/TransformComponent.h"
#include "Gameplay/Actor.h"
#include "Debug/Log.h"
#include "Gameplay/World.h"

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

TMatrix4x4F ACameraComponent::GetEnvironmentViewMatrix()
{
	TVector3F direction = GetOwner()->GetTransform()->GetDirection();

	TVector3F toTargetNorm = TVector3F(direction._x, direction._y, direction._z);

	TVector3F right = Cross({ 0.0f, 1.0f, 0.0f }, toTargetNorm).Normalized();
	TVector3F up = Cross(toTargetNorm, right).Normalized();

	return TMatrix4x4F(
		right._x, right._y, right._z, 0.0f,
		up._x, up._y, up._z,0.0f,
		toTargetNorm._x, toTargetNorm._y, toTargetNorm._z,0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	);
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

void ACameraComponent::SetLayoutMask(unsigned mask)
{
	_viewLayoutMask = mask;
}

void ACameraComponent::SetLayoutMaskIndex(unsigned maskIndex)
{
	_viewLayoutMask |= 1 << maskIndex;
}

void ACameraComponent::SetLayoutMaskName(const FString& name)
{
	AWorld* world = GetWorld();
	if (!world)
		return;

	const THashMap<FString, unsigned char>& layous = world->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		SetLayoutMaskIndex(it->_second);
	else
		ErrorString("Layer" + name + " not defined in the world");
}

void ACameraComponent::SetLayoutMaskOutIndex(unsigned maskIndex)
{
	int tempViewLayoutMask = _viewLayoutMask;

	tempViewLayoutMask &= ~(1 << maskIndex);
	_viewLayoutMask &= tempViewLayoutMask;
}

void ACameraComponent::SetLayoutMaskOutName(const FString& name)
{
	AWorld* world = GetWorld();
	if (!world)
		return;

	const THashMap<FString, unsigned char>& layous = world->Layers();

	auto it = layous.Find(name);
	if (it != layous.End())
		SetLayoutMaskOutIndex(it->_second);
	else
		ErrorString("Layer" + name + " not defined in the world");
}

void ACameraComponent::SetLayoutMaskAll()
{
	_viewLayoutMask = M_MAX_UNSIGNED;
}

TMatrix3x4F ACameraComponent::EffectiveWorldTransform() const
{
	AActor* owner = GetOwner();
	ATransformComponent* transform = owner->GetTransform();
	TMatrix3x4F worldTransform(transform->GetWorldPosition(), transform->GetWorldRotation(), 1.0f);

	return worldTransform;
}


const TMatrix3x4F& ACameraComponent::GetViewMatrix()
{
	AActor* owner = GetOwner();
	ATransformComponent* transform = owner->GetTransform();

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