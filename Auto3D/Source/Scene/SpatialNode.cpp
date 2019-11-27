#include "SpatialNode.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

SpatialNode::SpatialNode() :
    _worldTransform(Matrix3x4F::IDENTITY),
    _position(Vector3F::ZERO),
    _rotation(Quaternion::IDENTITY),
    _scale(Vector3F::ONE)
{
    SetFlag(NF_SPATIAL, true);
}

void SpatialNode::RegisterObject()
{
    RegisterFactory<SpatialNode>();
    CopyBaseAttributes<SpatialNode, Node>();
    RegisterRefAttribute("position", &SpatialNode::GetPosition, &SpatialNode::SetPosition, Vector3F::ZERO);
    RegisterRefAttribute("rotation", &SpatialNode::GetRotation, &SpatialNode::SetRotation, Quaternion::IDENTITY);
    RegisterRefAttribute("scale", &SpatialNode::GetScale, &SpatialNode::SetScale, Vector3F::ONE);
}

void SpatialNode::SetPosition(const Vector3F& newPosition)
{
    _position = newPosition;
    OnTransformChanged();
}

void SpatialNode::SetRotation(const Quaternion& newRotation)
{
    _rotation = newRotation;
    OnTransformChanged();
}

void SpatialNode::SetDirection(const Vector3F& newDirection)
{
    _rotation = Quaternion(Vector3F::FORWARD, newDirection);
    OnTransformChanged();
}

void SpatialNode::SetScale(const Vector3F& newScale)
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

void SpatialNode::SetScale(float newScale)
{
    SetScale(Vector3F(newScale, newScale, newScale));
}

void SpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation)
{
    _position = newPosition;
    _rotation = newRotation;
    OnTransformChanged();
}

void SpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale)
{
    _position = newPosition;
    _rotation = newRotation;
    _scale = newScale;
    OnTransformChanged();
}

void SpatialNode::SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale)
{
    SetTransform(newPosition, newRotation, Vector3F(newScale, newScale, newScale));
}

void SpatialNode::SetWorldPosition(const Vector3F& newPosition)
{
    SpatialNode* parentNode = GetSpatialParent();
    SetPosition(parentNode ? parentNode->GetWorldTransform().Inverse() * newPosition : newPosition);
}

void SpatialNode::SetWorldRotation(const Quaternion& newRotation)
{
    SpatialNode* parentNode = GetSpatialParent();
    SetRotation(parentNode ? parentNode->GetWorldRotation().Inverse() * newRotation : newRotation);
}

void SpatialNode::SetWorldDirection(const Vector3F& newDirection)
{
    SpatialNode* parentNode = GetSpatialParent();
    SetDirection(parentNode ? parentNode->GetWorldRotation().Inverse() * newDirection : newDirection);
}

void SpatialNode::SetWorldScale(const Vector3F& newScale)
{
    SpatialNode* parentNode = GetSpatialParent();
    SetScale(parentNode ? newScale / parentNode->GetWorldScale() : newScale);
}

void SpatialNode::SetWorldScale(float newScale)
{
    SetWorldScale(Vector3F(newScale, newScale, newScale));
}

void SpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation)
{
    SpatialNode* parentNode = GetSpatialParent();
    if (parentNode)
    {
        Vector3F localPosition = parentNode->GetWorldTransform().Inverse() * newPosition;
        Quaternion localRotation = parentNode->GetWorldRotation().Inverse() * newRotation;
        SetTransform(localPosition, localRotation);
    }
    else
        SetTransform(newPosition, newRotation);
}

void SpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale)
{
    SpatialNode* parentNode = GetSpatialParent();
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

void SpatialNode::SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale)
{
    SetWorldTransform(newPosition, newRotation, Vector3F(newScale, newScale, newScale));
}

void SpatialNode::Translate(const Vector3F& delta, TransformSpace::Type space)
{
    SpatialNode* parentNode = GetSpatialParent();

    switch (space)
    {
    case TransformSpace::LOCAL:
        // Note: local space translation disregards local scale for scale-independent movement speed
        _position += _rotation * delta;
        break;

    case TransformSpace::PARENT:
        _position += delta;
        break;

    case TransformSpace::WORLD:
        _position += !parentNode ? delta : parentNode->GetWorldTransform().Inverse() * Vector4F(delta, 0.0f);
        break;
    }

    OnTransformChanged();
}

void SpatialNode::Rotate(const Quaternion& delta, TransformSpace::Type space)
{
    SpatialNode* parentNode = GetSpatialParent();
    
    switch (space)
    {
    case TransformSpace::LOCAL:
        _rotation = (_rotation * delta).Normalized();
        break;

    case TransformSpace::PARENT:
        _rotation = (delta * _rotation).Normalized();
        break;

    case TransformSpace::WORLD:
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

void SpatialNode::RotateAround(const Vector3F& point, const Quaternion& delta, TransformSpace::Type space)
{
    SpatialNode* parentNode = GetSpatialParent();
    Vector3F parentSpacePoint;
    Quaternion oldRotation = _rotation;

    switch (space)
    {
    case TransformSpace::LOCAL:
        parentSpacePoint = GetTransform() * point;
        _rotation = (_rotation * delta).Normalized();
        break;

    case TransformSpace::PARENT:
        parentSpacePoint = point;
        _rotation = (delta * _rotation).Normalized();
        break;

    case TransformSpace::WORLD:
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

void SpatialNode::Yaw(float angle, TransformSpace::Type space)
{
    Rotate(Quaternion(angle, Vector3F::UP), space);
}

void SpatialNode::Pitch(float angle, TransformSpace::Type space)
{
    Rotate(Quaternion(angle, Vector3F::RIGHT), space);
}

void SpatialNode::Roll(float angle, TransformSpace::Type space)
{
    Rotate(Quaternion(angle, Vector3F::FORWARD), space);
}

bool SpatialNode::LookAt(const Vector3F& target, const Vector3F& up, TransformSpace::Type space)
{
    SpatialNode* parentNode = GetSpatialParent();
    Vector3F worldSpaceTarget;

    switch (space)
    {
    case TransformSpace::LOCAL:
        worldSpaceTarget = GetWorldTransform() * target;
        break;

    case TransformSpace::PARENT:
        worldSpaceTarget = !parentNode ? target : parentNode->GetWorldTransform() * target;
        break;

    case TransformSpace::WORLD:
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

void SpatialNode::ApplyScale(float delta)
{
    ApplyScale(Vector3F(delta, delta, delta));
}

void SpatialNode::ApplyScale(const Vector3F& delta)
{
    _scale *= delta;
    OnTransformChanged();
}

void SpatialNode::OnParentSet(Node* newParent, Node*)
{
    SetFlag(NF_SPATIAL_PARENT, dynamic_cast<SpatialNode*>(newParent) != 0);
    OnTransformChanged();
}

void SpatialNode::OnTransformChanged()
{
    SetFlag(NF_WORLD_TRANSFORM_DIRTY, true);

    const Vector<SharedPtr<Node> >& children = Children();
    for (auto it = children.Begin(); it != children.End(); ++it)
    {
        Node* child = *it;
        if (child->TestFlag(NF_SPATIAL))
            static_cast<SpatialNode*>(child)->OnTransformChanged();
    }
}

void SpatialNode::UpdateWorldTransform() const
{
    if (TestFlag(NF_SPATIAL_PARENT))
        _worldTransform = static_cast<SpatialNode*>(Parent())->GetWorldTransform() * Matrix3x4F(_position, _rotation, _scale);
    else
        _worldTransform = Matrix3x4F(_position, _rotation, _scale);
    SetFlag(NF_WORLD_TRANSFORM_DIRTY, false);
}

}
