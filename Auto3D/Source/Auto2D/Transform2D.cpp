#include "Transform2D.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{


ATransform2D::ATransform2D() :
	_worldTransform(TMatrix3x4F::IDENTITY),
	_position(TVector3F::ZERO),
	_rotation(FQuaternion::IDENTITY),
	_scale(TVector3F::ONE)
{
	SetFlag(NF_2D_SPATIAL, true);
}

void ATransform2D::SetPosition(const TVector3F& newPosition)
{
	_position = newPosition;
	OnTransformChanged();
}

void ATransform2D::SetRotation(const FQuaternion& newRotation)
{
	_rotation = newRotation;
	OnTransformChanged();
}

void ATransform2D::SetDirection(const TVector3F& newDirection)
{
	_rotation = FQuaternion(TVector3F::FORWARD, newDirection);
	OnTransformChanged();
}

void ATransform2D::SetScale(const TVector3F& newScale)
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

void ATransform2D::SetScale(float newScale)
{
	SetScale(TVector3F(newScale, newScale, newScale));
}

void ATransform2D::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
	_position = newPosition;
	_rotation = newRotation;
	OnTransformChanged();
}

void ATransform2D::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
	_position = newPosition;
	_rotation = newRotation;
	_scale = newScale;
	OnTransformChanged();
}

void ATransform2D::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
	SetTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ATransform2D::SetWorldPosition(const TVector3F& newPosition)
{
	ATransform2D* parentNode = GetSpatialParent();
	SetPosition(parentNode ? parentNode->GetWorldTransform().Inverse() * newPosition : newPosition);
}

void ATransform2D::SetWorldRotation(const FQuaternion& newRotation)
{
	ATransform2D* parentNode = GetSpatialParent();
	SetRotation(parentNode ? parentNode->GetWorldRotation().Inverse() * newRotation : newRotation);
}

void ATransform2D::SetWorldDirection(const TVector3F& newDirection)
{
	ATransform2D* parentNode = GetSpatialParent();
	SetDirection(parentNode ? parentNode->GetWorldRotation().Inverse() * newDirection : newDirection);
}

void ATransform2D::SetWorldScale(const TVector3F& newScale)
{
	ATransform2D* parentNode = GetSpatialParent();
	SetScale(parentNode ? newScale / parentNode->GetWorldScale() : newScale);
}

void ATransform2D::SetWorldScale(float newScale)
{
	SetWorldScale(TVector3F(newScale, newScale, newScale));
}

void ATransform2D::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
	ATransform2D* parentNode = GetSpatialParent();
	if (parentNode)
	{
		TVector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
		FQuaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
		SetTransform(localPosition, localRotation);
	}
	else
		SetTransform(newPosition, newRotation);
}

void ATransform2D::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
	ATransform2D* parentNode = GetSpatialParent();
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

void ATransform2D::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
	SetWorldTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ATransform2D::Translate(const TVector3F& delta, ETransform2DSpace::Type space)
{
	ATransform2D* parentNode = GetSpatialParent();

	switch (space)
	{
	case ETransform2DSpace::LOCAL:
		// Note: local space translation disregards local scale for scale-independent movement speed
		_position += _rotation * delta;
		break;

	case ETransform2DSpace::PARENT:
		_position += delta;
		break;

	case ETransform2DSpace::WORLD:
		_position += !parentNode ? delta : parentNode->GetWorldTransform().Inverse() * TVector4F(delta, 0.0f);
		break;
	}

	OnTransformChanged();
}

void ATransform2D::Rotate(const FQuaternion& delta, ETransform2DSpace::Type space)
{
	ATransform2D* parentNode = GetSpatialParent();

	switch (space)
	{
	case ETransform2DSpace::LOCAL:
		_rotation = (_rotation * delta).Normalized();
		break;

	case ETransform2DSpace::PARENT:
		_rotation = (delta * _rotation).Normalized();
		break;

	case ETransform2DSpace::WORLD:
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

void ATransform2D::RotateAround(const TVector3F& point, const FQuaternion& delta, ETransform2DSpace::Type space)
{
	ATransform2D* parentNode = GetSpatialParent();
	TVector3F parentSpacePoint;
	FQuaternion oldRotation = _rotation;

	switch (space)
	{
	case ETransform2DSpace::LOCAL:
		parentSpacePoint = GetTransform() * point;
		_rotation = (_rotation * delta).Normalized();
		break;

	case ETransform2DSpace::PARENT:
		parentSpacePoint = point;
		_rotation = (delta * _rotation).Normalized();
		break;

	case ETransform2DSpace::WORLD:
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

void ATransform2D::Yaw(float angle, ETransform2DSpace::Type space)
{
	Rotate(FQuaternion(angle, TVector3F::UP), space);
}

void ATransform2D::Pitch(float angle, ETransform2DSpace::Type space)
{
	Rotate(FQuaternion(angle, TVector3F::RIGHT), space);
}

void ATransform2D::Roll(float angle, ETransform2DSpace::Type space)
{
	Rotate(FQuaternion(angle, TVector3F::FORWARD), space);
}

bool ATransform2D::LookAt(const TVector3F& target, const TVector3F& up, ETransform2DSpace::Type space)
{
	ATransform2D* parentNode = GetSpatialParent();
	TVector3F worldSpaceTarget;

	switch (space)
	{
	case ETransform2DSpace::LOCAL:
		worldSpaceTarget = GetWorldTransform() * target;
		break;

	case ETransform2DSpace::PARENT:
		worldSpaceTarget = !parentNode ? target : parentNode->GetWorldTransform() * target;
		break;

	case ETransform2DSpace::WORLD:
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

void ATransform2D::ApplyScale(float delta)
{
	ApplyScale(TVector3F(delta, delta, delta));
}

void ATransform2D::ApplyScale(const TVector3F& delta)
{
	_scale *= delta;
	OnTransformChanged();
}

void ATransform2D::OnParentSet(ANode2D* newParent, ANode2D*)
{
	SetFlag(NF_2D_SPATIAL_PARENT, dynamic_cast<ATransform2D*>(newParent) != 0);
	OnTransformChanged();
}

void ATransform2D::OnTransformChanged()
{
	SetFlag(NF_2D_WORLD_TRANSFORM_DIRTY, true);

	const TVector<TSharedPtr<ANode2D> >& children = Children();
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		ANode2D* child = *it;
		if (child->TestFlag(NF_2D_SPATIAL))
			static_cast<ATransform2D*>(child)->OnTransformChanged();
	}
}

void ATransform2D::UpdateWorldTransform() const
{
	if (TestFlag(NF_2D_SPATIAL_PARENT))
		_worldTransform = static_cast<ATransform2D*>(Parent())->GetWorldTransform() * TMatrix3x4F(_position, _rotation, _scale);
	else
		_worldTransform = TMatrix3x4F(_position, _rotation, _scale);
	SetFlag(NF_2D_WORLD_TRANSFORM_DIRTY, false);
}

}
