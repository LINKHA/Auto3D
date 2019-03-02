#include "UISpatialNode.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

UISpatialNode::UISpatialNode() :
	_worldTransform(Matrix3x4F::IDENTITY),
	_position(Vector3F::ZERO),
	_rotation(Quaternion::IDENTITY),
	_scale(Vector3F::ONE)
{
	SetFlag(UNF_SPATIAL, true);
}

void UISpatialNode::RegisterObject()
{
	RegisterFactory<UISpatialNode>();
	CopyBaseAttributes<UISpatialNode, UINode>();
	RegisterRefAttribute("position", &UISpatialNode::GetPosition, &UISpatialNode::SetPosition, Vector3F::ZERO);
	RegisterRefAttribute("rotation", &UISpatialNode::GetRotation, &UISpatialNode::SetRotation, Quaternion::IDENTITY);
	RegisterRefAttribute("scale", &UISpatialNode::GetScale, &UISpatialNode::SetScale, Vector3F::ONE);
}

void UISpatialNode::SetPosition(const Vector3F& newPosition)
{
	_position = newPosition;
	OnTransformChanged();
}

void UISpatialNode::SetRotation(const Quaternion& newRotation)
{
	_rotation = newRotation;
	OnTransformChanged();
}

void UISpatialNode::SetDirection(const Vector3F& newDirection)
{
	_rotation = Quaternion(Vector3F::FORWARD, newDirection);
	OnTransformChanged();
}

void UISpatialNode::SetScale(const Vector3F& newScale)
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

void UISpatialNode::SetScale(float newScale)
{
	SetScale(Vector3F(newScale, newScale, newScale));
}

void UISpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation)
{
	_position = newPosition;
	_rotation = newRotation;
	OnTransformChanged();
}

void UISpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale)
{
	_position = newPosition;
	_rotation = newRotation;
	_scale = newScale;
	OnTransformChanged();
}

void UISpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale)
{
	SetTransform(newPosition, newRotation, Vector3F(newScale, newScale, newScale));
}

void UISpatialNode::SetWorldPosition(const Vector3F& newPosition)
{
	UISpatialNode* parentNode = GetSpatialParent();
	SetPosition(parentNode ? parentNode->GetWorldTransform().Inverse() * newPosition : newPosition);
}

void UISpatialNode::SetWorldRotation(const Quaternion& newRotation)
{
	UISpatialNode* parentNode = GetSpatialParent();
	SetRotation(parentNode ? parentNode->GetWorldRotation().Inverse() * newRotation : newRotation);
}

void UISpatialNode::SetWorldDirection(const Vector3F& newDirection)
{
	UISpatialNode* parentNode = GetSpatialParent();
	SetDirection(parentNode ? parentNode->GetWorldRotation().Inverse() * newDirection : newDirection);
}

void UISpatialNode::SetWorldScale(const Vector3F& newScale)
{
	UISpatialNode* parentNode = GetSpatialParent();
	SetScale(parentNode ? newScale / parentNode->GetWorldScale() : newScale);
}

void UISpatialNode::SetWorldScale(float newScale)
{
	SetWorldScale(Vector3F(newScale, newScale, newScale));
}

void UISpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation)
{
	UISpatialNode* parentNode = GetSpatialParent();
	if (parentNode)
	{
		Vector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
		Quaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
		SetTransform(localPosition, localRotation);
	}
	else
		SetTransform(newPosition, newRotation);
}

void UISpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale)
{
	UISpatialNode* parentNode = GetSpatialParent();
	if (parentNode)
	{
		Vector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
		Quaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
		Vector3F localScale = newScale / parentNode->GetWorldScale();
		SetTransform(localPosition, localRotation, localScale);
	}
	else
		SetTransform(newPosition, newRotation);
}

void UISpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale)
{
	SetWorldTransform(newPosition, newRotation, Vector3F(newScale, newScale, newScale));
}

void UISpatialNode::Translate(const Vector3F& delta, UITransformSpace space)
{
	UISpatialNode* parentNode = GetSpatialParent();

	switch (space)
	{
	case UITransformSpace::LOCAL:
		// Note: local space translation disregards local scale for scale-independent movement speed
		_position += _rotation * delta;
		break;

	case UITransformSpace::PARENT:
		_position += delta;
		break;

	case UITransformSpace::WORLD:
		_position += !parentNode ? delta : parentNode->GetWorldTransform().Inverse() * Vector4F(delta, 0.0f);
		break;
	}

	OnTransformChanged();
}

void UISpatialNode::Rotate(const Quaternion& delta, UITransformSpace space)
{
	UISpatialNode* parentNode = GetSpatialParent();

	switch (space)
	{
	case UITransformSpace::LOCAL:
		_rotation = (_rotation * delta).Normalized();
		break;

	case UITransformSpace::PARENT:
		_rotation = (delta * _rotation).Normalized();
		break;

	case UITransformSpace::WORLD:
		if (!parentNode)
			_rotation = (delta * _rotation).Normalized();
		else
		{
			Quaternion worldRotation = GetWorldRotation();
			_rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
		}
		break;
	}

	OnTransformChanged();
}

void UISpatialNode::RotateAround(const Vector3F& point, const Quaternion& delta, UITransformSpace space)
{
	UISpatialNode* parentNode = GetSpatialParent();
	Vector3F parentSpacePoint;
	Quaternion oldRotation = _rotation;

	switch (space)
	{
	case UITransformSpace::LOCAL:
		parentSpacePoint = GetTransform() * point;
		_rotation = (_rotation * delta).Normalized();
		break;

	case UITransformSpace::PARENT:
		parentSpacePoint = point;
		_rotation = (delta * _rotation).Normalized();
		break;

	case UITransformSpace::WORLD:
		if (!parentNode)
		{
			parentSpacePoint = point;
			_rotation = (delta * _rotation).Normalized();
		}
		else
		{
			parentSpacePoint = parentNode->GetWorldTransform().Inverse() * point;
			Quaternion worldRotation = GetWorldRotation();
			_rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
		}
		break;
	}

	Vector3F oldRelativePos = oldRotation.Inverse() * (_position - parentSpacePoint);
	_position = _rotation * oldRelativePos + parentSpacePoint;

	OnTransformChanged();
}

void UISpatialNode::Yaw(float angle, UITransformSpace space)
{
	Rotate(Quaternion(angle, Vector3F::UP), space);
}

void UISpatialNode::Pitch(float angle, UITransformSpace space)
{
	Rotate(Quaternion(angle, Vector3F::RIGHT), space);
}

void UISpatialNode::Roll(float angle, UITransformSpace space)
{
	Rotate(Quaternion(angle, Vector3F::FORWARD), space);
}

bool UISpatialNode::LookAt(const Vector3F& target, const Vector3F& up, UITransformSpace space)
{
	UISpatialNode* parentNode = GetSpatialParent();
	Vector3F worldSpaceTarget;

	switch (space)
	{
	case UITransformSpace::LOCAL:
		worldSpaceTarget = GetWorldTransform() * target;
		break;

	case UITransformSpace::PARENT:
		worldSpaceTarget = !parentNode ? target : parentNode->GetWorldTransform() * target;
		break;

	case UITransformSpace::WORLD:
		worldSpaceTarget = target;
		break;
	}

	Vector3F lookDir = worldSpaceTarget - GetWorldPosition();
	// Check if target is very close, in that case can not reliably calculate lookat direction
	if (lookDir.Equals(Vector3F::ZERO))
		return false;
	Quaternion newRotation;
	// Do nothing if setting look rotation failed
	if (!newRotation.FromLookRotation(lookDir, up))
		return false;

	SetWorldRotation(newRotation);
	return true;
}

void UISpatialNode::ApplyScale(float delta)
{
	ApplyScale(Vector3F(delta, delta, delta));
}

void UISpatialNode::ApplyScale(const Vector3F& delta)
{
	_scale *= delta;
	OnTransformChanged();
}

void UISpatialNode::OnParentSet(UINode* newParent, UINode*)
{
	SetFlag(UNF_SPATIAL_PARENT, dynamic_cast<UISpatialNode*>(newParent) != 0);
	OnTransformChanged();
}

void UISpatialNode::OnTransformChanged()
{
	SetFlag(UNF_WORLD_TRANSFORM_DIRTY, true);

	const Vector<SharedPtr<UINode> >& children = Children();
	for (auto it = children.Begin(); it != children.End(); ++it)
	{
		UINode* child = *it;
		if (child->TestFlag(UNF_SPATIAL))
			static_cast<UISpatialNode*>(child)->OnTransformChanged();
	}
}

void UISpatialNode::UpdateWorldTransform() const
{
	if (TestFlag(UNF_SPATIAL_PARENT))
		_worldTransform = static_cast<UISpatialNode*>(Parent())->GetWorldTransform() * Matrix3x4F(_position, _rotation, _scale);
	else
		_worldTransform = Matrix3x4F(_position, _rotation, _scale);
	SetFlag(UNF_WORLD_TRANSFORM_DIRTY, false);
}

}
