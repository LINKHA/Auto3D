#include "Component/Transform.h"
#include "Gameplay/Actor.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

ATransform::ATransform() :
    _worldTransform(TMatrix3x4F::IDENTITY),
    _position(TVector3F::ZERO),
    _rotation(FQuaternion::IDENTITY),
    _scale(TVector3F::ONE)
{
}

void ATransform::BeginPlay()
{
	Super::BeginPlay();
}

void ATransform::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

void ATransform::SetPosition(const TVector3F& newPosition)
{
    _position = newPosition;
    OnTransformChanged();
}

void ATransform::SetRotation(const FQuaternion& newRotation)
{
    _rotation = newRotation;
    OnTransformChanged();
}

void ATransform::SetDirection(const TVector3F& newDirection)
{
    _rotation = FQuaternion(TVector3F::FORWARD, newDirection);
    OnTransformChanged();
}

void ATransform::SetScale(float newScale)
{
	SetScale(TVector3F(newScale, newScale, newScale));
}

void ATransform::SetScale(const TVector3F& newScale)
{
    _scale = newScale;
    // Make sure scale components never go to exactly zero, to prevent problems with decomposing the world matrix
    if (_scale._x == 0.0f)
        _scale._x = M_EPSILON;
    if (_scale._y == 0.0f)
        _scale._y = M_EPSILON;
    if (_scale._z == 0.0f)
        _scale._z = M_EPSILON;

    OnTransformChanged();
}

void ATransform::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
    _position = newPosition;
    _rotation = newRotation;
    OnTransformChanged();
}

void ATransform::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
    _position = newPosition;
    _rotation = newRotation;
    _scale = newScale;
    OnTransformChanged();
}

void ATransform::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
    SetTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ATransform::SetWorldPosition(const TVector3F& newPosition)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    SetPosition(parentNode ? parentNode->GetWorldTransform().Inverse() * newPosition : newPosition);
}

void ATransform::SetWorldRotation(const FQuaternion& newRotation)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    SetRotation(parentNode ? parentNode->GetWorldRotation().Inverse() * newRotation : newRotation);
}

void ATransform::SetWorldDirection(const TVector3F& newDirection)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    SetDirection(parentNode ? parentNode->GetWorldRotation().Inverse() * newDirection : newDirection);
}

void ATransform::SetWorldScale(const TVector3F& newScale)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    SetScale(parentNode ? newScale / parentNode->GetWorldScale() : newScale);
}

void ATransform::SetWorldScale(float newScale)
{
    SetWorldScale(TVector3F(newScale, newScale, newScale));
}

void ATransform::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    if (parentNode)
    {
        TVector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
        FQuaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
        SetTransform(localPosition, localRotation);
    }
    else
        SetTransform(newPosition, newRotation);
}

void ATransform::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    if (parentNode)
    {
        TVector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
        FQuaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
        TVector3F localScale = newScale / parentNode->GetWorldScale();
        SetTransform(localPosition, localRotation, localScale);
    }
    else
        SetTransform(newPosition, newRotation);
}

void ATransform::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
    SetWorldTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ATransform::Translate(const TVector3F& delta, ETransformSpace::Type space)
{
    SPtr<ATransform> parentNode = GetParentTransform();

    switch (space)
    {
    case ETransformSpace::LOCAL:
        // Note: local space translation disregards local scale for scale-independent movement speed
        _position += _rotation * delta;
        break;

    case ETransformSpace::PARENT:
        _position += delta;
        break;

    case ETransformSpace::WORLD:
        _position += !parentNode ? delta : parentNode->GetWorldTransform().Inverse() * TVector4F(delta, 0.0f);
        break;
    }

    OnTransformChanged();
}

void ATransform::Rotate(const FQuaternion& delta, ETransformSpace::Type space)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    
    switch (space)
    {
    case ETransformSpace::LOCAL:
        _rotation = (_rotation * delta).Normalized();
        break;

    case ETransformSpace::PARENT:
        _rotation = (delta * _rotation).Normalized();
        break;

    case ETransformSpace::WORLD:
        if (!parentNode)
            _rotation = (delta * _rotation).Normalized();
        else
        {
            FQuaternion worldRotation = GetWorldRotation();
            _rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
        }
        break;
    }

    OnTransformChanged();
}

void ATransform::RotateAround(const TVector3F& point, const FQuaternion& delta, ETransformSpace::Type space)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    TVector3F parentSpacePoint;
    FQuaternion oldRotation = _rotation;

    switch (space)
    {
    case ETransformSpace::LOCAL:
        parentSpacePoint = GetTransform() * point;
        _rotation = (_rotation * delta).Normalized();
        break;

    case ETransformSpace::PARENT:
        parentSpacePoint = point;
        _rotation = (delta * _rotation).Normalized();
        break;

    case ETransformSpace::WORLD:
        if (!parentNode)
        {
            parentSpacePoint = point;
            _rotation = (delta * _rotation).Normalized();
        }
        else
        {
            parentSpacePoint = parentNode->GetWorldTransform().Inverse() * point;
            FQuaternion worldRotation = GetWorldRotation();
            _rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
        }
        break;
    }

    TVector3F oldRelativePos = oldRotation.Inverse() * (_position - parentSpacePoint);
    _position = _rotation * oldRelativePos + parentSpacePoint;

    OnTransformChanged();
}

void ATransform::Yaw(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::UP), space);
}

void ATransform::Pitch(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::RIGHT), space);
}

void ATransform::Roll(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::FORWARD), space);
}

bool ATransform::LookAt(const TVector3F& target, const TVector3F& up, ETransformSpace::Type space)
{
    SPtr<ATransform> parentNode = GetParentTransform();
    TVector3F worldSpaceTarget;

    switch (space)
    {
    case ETransformSpace::LOCAL:
        worldSpaceTarget = GetWorldTransform() * target;
        break;

    case ETransformSpace::PARENT:
        worldSpaceTarget = !parentNode ? target : parentNode->GetWorldTransform() * target;
        break;

    case ETransformSpace::WORLD:
        worldSpaceTarget = target;
        break;
    }

    TVector3F lookDir = worldSpaceTarget - GetWorldPosition();
    // Check if target is very close, in that case can not reliably calculate lookat direction
    if (lookDir.Equals(TVector3F::ZERO))
        return false;
    FQuaternion newRotation;
    // Do nothing if setting look rotation failed
    if (!newRotation.FromLookRotation(lookDir, up))
        return false;

    SetWorldRotation(newRotation);
    return true;
}

void ATransform::ApplyScale(float delta)
{
    ApplyScale(TVector3F(delta, delta, delta));
}

void ATransform::ApplyScale(const TVector3F& delta)
{
    _scale *= delta;
    OnTransformChanged();
}

SPtr<ATransform> ATransform::GetParentTransform() const
{ 
	auto owner = GetOwner();
	return owner->TestFlag(NF_SPATIAL_PARENT) ? owner->GetParentNode()->GetTransform() : nullptr;
}

const TMatrix3x4F& ATransform::GetWorldTransform() const
{
	auto owner = GetOwner();

	if (owner->TestFlag(NF_WORLD_TRANSFORM_DIRTY)) 
		UpdateWorldTransform();

	return _worldTransform; 
}

void ATransform::OnTransformChanged()
{
	SPtr<AActor> owner = GetOwner();
	owner->SetFlag(NF_WORLD_TRANSFORM_DIRTY, true);

	UpdateWorldTransform();

	const TVector<SPtr<AActor>>& children = GetOwner()->GetChildrenNode();
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		SPtr<AActor> child = *it;
		if (child->TestFlag(NF_SPATIAL))
			child->GetTransform()->OnTransformChanged();
		
	}
}

void ATransform::UpdateWorldTransform() const
{
	auto owner = GetOwner();

    if (owner->TestFlag(NF_SPATIAL_PARENT))
        _worldTransform = GetParentTransform()->GetWorldTransform() * TMatrix3x4F(_position, _rotation, _scale);
    else
        _worldTransform = TMatrix3x4F(_position, _rotation, _scale);
	owner->SetFlag(NF_WORLD_TRANSFORM_DIRTY, false);
}

}
