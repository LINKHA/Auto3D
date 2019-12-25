#include "SpatialNode.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

REGISTER_CLASS
{
	REGISTER_CALSS_FACTORY_IMP(ASpatialNode)
	.constructor<>()
	.property("position", &ASpatialNode::GetPosition, &ASpatialNode::SetPosition)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("rotation", &ASpatialNode::GetRotation, &ASpatialNode::SetRotation)
	(
		metadata(SERIALIZABLE, "")
	)
	.property("scale", &ASpatialNode::GetScale, static_cast<void(ASpatialNode::*)(const TVector3F&)>(&ASpatialNode::SetScale))
	(
		metadata(SERIALIZABLE, "")
	)
	;
}

ASpatialNode::ASpatialNode() :
    _worldTransform(TMatrix3x4F::IDENTITY),
    _position(TVector3F::ZERO),
    _rotation(FQuaternion::IDENTITY),
    _scale(TVector3F::ONE)
{
    SetFlag(NF_SPATIAL, true);
}

void ASpatialNode::SetPosition(const TVector3F& newPosition)
{
    _position = newPosition;
    OnTransformChanged();
}

void ASpatialNode::SetRotation(const FQuaternion& newRotation)
{
    _rotation = newRotation;
    OnTransformChanged();
}

void ASpatialNode::SetDirection(const TVector3F& newDirection)
{
    _rotation = FQuaternion(TVector3F::FORWARD, newDirection);
    OnTransformChanged();
}

void ASpatialNode::SetScale(float newScale)
{
	SetScale(TVector3F(newScale, newScale, newScale));
}

void ASpatialNode::SetScale(const TVector3F& newScale)
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

void ASpatialNode::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
    _position = newPosition;
    _rotation = newRotation;
    OnTransformChanged();
}

void ASpatialNode::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
    _position = newPosition;
    _rotation = newRotation;
    _scale = newScale;
    OnTransformChanged();
}

void ASpatialNode::SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
    SetTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ASpatialNode::SetWorldPosition(const TVector3F& newPosition)
{
    ASpatialNode* parentNode = GetSpatialParent();
    SetPosition(parentNode ? parentNode->GetWorldTransform().Inverse() * newPosition : newPosition);
}

void ASpatialNode::SetWorldRotation(const FQuaternion& newRotation)
{
    ASpatialNode* parentNode = GetSpatialParent();
    SetRotation(parentNode ? parentNode->GetWorldRotation().Inverse() * newRotation : newRotation);
}

void ASpatialNode::SetWorldDirection(const TVector3F& newDirection)
{
    ASpatialNode* parentNode = GetSpatialParent();
    SetDirection(parentNode ? parentNode->GetWorldRotation().Inverse() * newDirection : newDirection);
}

void ASpatialNode::SetWorldScale(const TVector3F& newScale)
{
    ASpatialNode* parentNode = GetSpatialParent();
    SetScale(parentNode ? newScale / parentNode->GetWorldScale() : newScale);
}

void ASpatialNode::SetWorldScale(float newScale)
{
    SetWorldScale(TVector3F(newScale, newScale, newScale));
}

void ASpatialNode::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation)
{
    ASpatialNode* parentNode = GetSpatialParent();
    if (parentNode)
    {
        TVector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
        FQuaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
        SetTransform(localPosition, localRotation);
    }
    else
        SetTransform(newPosition, newRotation);
}

void ASpatialNode::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale)
{
    ASpatialNode* parentNode = GetSpatialParent();
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

void ASpatialNode::SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale)
{
    SetWorldTransform(newPosition, newRotation, TVector3F(newScale, newScale, newScale));
}

void ASpatialNode::Translate(const TVector3F& delta, ETransformSpace::Type space)
{
    ASpatialNode* parentNode = GetSpatialParent();

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

void ASpatialNode::Rotate(const FQuaternion& delta, ETransformSpace::Type space)
{
    ASpatialNode* parentNode = GetSpatialParent();
    
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

void ASpatialNode::RotateAround(const TVector3F& point, const FQuaternion& delta, ETransformSpace::Type space)
{
    ASpatialNode* parentNode = GetSpatialParent();
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

void ASpatialNode::Yaw(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::UP), space);
}

void ASpatialNode::Pitch(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::RIGHT), space);
}

void ASpatialNode::Roll(float angle, ETransformSpace::Type space)
{
    Rotate(FQuaternion(angle, TVector3F::FORWARD), space);
}

bool ASpatialNode::LookAt(const TVector3F& target, const TVector3F& up, ETransformSpace::Type space)
{
    ASpatialNode* parentNode = GetSpatialParent();
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

void ASpatialNode::ApplyScale(float delta)
{
    ApplyScale(TVector3F(delta, delta, delta));
}

void ASpatialNode::ApplyScale(const TVector3F& delta)
{
    _scale *= delta;
    OnTransformChanged();
}

void ASpatialNode::OnParentSet(ANode* newParent, ANode*)
{
    SetFlag(NF_SPATIAL_PARENT, dynamic_cast<ASpatialNode*>(newParent) != 0);
    OnTransformChanged();
}

void ASpatialNode::OnTransformChanged()
{
    SetFlag(NF_WORLD_TRANSFORM_DIRTY, true);

    const TVector<TSharedPtr<ANode> >& children = Children();
    for (auto it = children.Begin(); it != children.End(); ++it)
    {
        ANode* child = *it;
        if (child->TestFlag(NF_SPATIAL))
            static_cast<ASpatialNode*>(child)->OnTransformChanged();
    }
}

void ASpatialNode::UpdateWorldTransform() const
{
    if (TestFlag(NF_SPATIAL_PARENT))
        _worldTransform = static_cast<ASpatialNode*>(Parent())->GetWorldTransform() * TMatrix3x4F(_position, _rotation, _scale);
    else
        _worldTransform = TMatrix3x4F(_position, _rotation, _scale);
    SetFlag(NF_WORLD_TRANSFORM_DIRTY, false);
}

}
