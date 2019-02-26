#include "SpatialNode.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

SpatialNode::SpatialNode() :
    _worldTransform(Matrix3x4::IDENTITY),
    _position(Vector3::ZERO),
    _rotation(Quaternion::IDENTITY),
    _scale(Vector3::ONE)
{
    SetFlag(NF_SPATIAL, true);
}

void SpatialNode::RegisterObject()
{
    RegisterFactory<SpatialNode>();
    CopyBaseAttributes<SpatialNode, Node>();
    RegisterRefAttribute("position", &SpatialNode::Position, &SpatialNode::SetPosition, Vector3::ZERO);
    RegisterRefAttribute("rotation", &SpatialNode::Rotation, &SpatialNode::SetRotation, Quaternion::IDENTITY);
    RegisterRefAttribute("scale", &SpatialNode::Scale, &SpatialNode::SetScale, Vector3::ONE);
}

void SpatialNode::SetPosition(const Vector3& newPosition)
{
    _position = newPosition;
    OnTransformChanged();
}

void SpatialNode::SetRotation(const Quaternion& newRotation)
{
    _rotation = newRotation;
    OnTransformChanged();
}

void SpatialNode::SetDirection(const Vector3& newDirection)
{
    _rotation = Quaternion(Vector3::FORWARD, newDirection);
    OnTransformChanged();
}

void SpatialNode::SetScale(const Vector3& newScale)
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
    SetScale(Vector3(newScale, newScale, newScale));
}

void SpatialNode::SetTransform(const Vector3& newPosition, const Quaternion& newRotation)
{
    _position = newPosition;
    _rotation = newRotation;
    OnTransformChanged();
}

void SpatialNode::SetTransform(const Vector3& newPosition, const Quaternion& newRotation, const Vector3& newScale)
{
    _position = newPosition;
    _rotation = newRotation;
    _scale = newScale;
    OnTransformChanged();
}

void SpatialNode::SetTransform(const Vector3& newPosition, const Quaternion& newRotation, float newScale)
{
    SetTransform(newPosition, newRotation, Vector3(newScale, newScale, newScale));
}

void SpatialNode::SetWorldPosition(const Vector3& newPosition)
{
    SpatialNode* parentNode = SpatialParent();
    SetPosition(parentNode ? parentNode->WorldTransform().Inverse() * newPosition : newPosition);
}

void SpatialNode::SetWorldRotation(const Quaternion& newRotation)
{
    SpatialNode* parentNode = SpatialParent();
    SetRotation(parentNode ? parentNode->WorldRotation().Inverse() * newRotation : newRotation);
}

void SpatialNode::SetWorldDirection(const Vector3& newDirection)
{
    SpatialNode* parentNode = SpatialParent();
    SetDirection(parentNode ? parentNode->WorldRotation().Inverse() * newDirection : newDirection);
}

void SpatialNode::SetWorldScale(const Vector3& newScale)
{
    SpatialNode* parentNode = SpatialParent();
    SetScale(parentNode ? newScale / parentNode->WorldScale() : newScale);
}

void SpatialNode::SetWorldScale(float newScale)
{
    SetWorldScale(Vector3(newScale, newScale, newScale));
}

void SpatialNode::SetWorldTransform(const Vector3& newPosition, const Quaternion& newRotation)
{
    SpatialNode* parentNode = SpatialParent();
    if (parentNode)
    {
        Vector3 localPosition = parentNode->WorldTransform().Inverse() * newPosition;
        Quaternion localRotation = parentNode->WorldRotation().Inverse() * newRotation;
        SetTransform(localPosition, localRotation);
    }
    else
        SetTransform(newPosition, newRotation);
}

void SpatialNode::SetWorldTransform(const Vector3& newPosition, const Quaternion& newRotation, const Vector3& newScale)
{
    SpatialNode* parentNode = SpatialParent();
    if (parentNode)
    {
        Vector3 localPosition = parentNode->WorldTransform().Inverse() * newPosition;
        Quaternion localRotation = parentNode->WorldRotation().Inverse() * newRotation;
        Vector3 localScale = newScale / parentNode->WorldScale();
        SetTransform(localPosition, localRotation, localScale);
    }
    else
        SetTransform(newPosition, newRotation);
}

void SpatialNode::SetWorldTransform(const Vector3& newPosition, const Quaternion& newRotation, float newScale)
{
    SetWorldTransform(newPosition, newRotation, Vector3(newScale, newScale, newScale));
}

void SpatialNode::Translate(const Vector3& delta, TransformSpace space)
{
    SpatialNode* parentNode = SpatialParent();

    switch (space)
    {
    case TS_LOCAL:
        // Note: local space translation disregards local scale for scale-independent movement speed
        _position += _rotation * delta;
        break;

    case TS_PARENT:
        _position += delta;
        break;

    case TS_WORLD:
        _position += !parentNode ? delta : parentNode->WorldTransform().Inverse() * Vector4(delta, 0.0f);
        break;
    }

    OnTransformChanged();
}

void SpatialNode::Rotate(const Quaternion& delta, TransformSpace space)
{
    SpatialNode* parentNode = SpatialParent();
    
    switch (space)
    {
    case TS_LOCAL:
        _rotation = (_rotation * delta).Normalized();
        break;

    case TS_PARENT:
        _rotation = (delta * _rotation).Normalized();
        break;

    case TS_WORLD:
        if (!parentNode)
            _rotation = (delta * _rotation).Normalized();
        else
        {
            Quaternion worldRotation = WorldRotation();
            _rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
        }
        break;
    }

    OnTransformChanged();
}

void SpatialNode::RotateAround(const Vector3& point, const Quaternion& delta, TransformSpace space)
{
    SpatialNode* parentNode = SpatialParent();
    Vector3 parentSpacePoint;
    Quaternion oldRotation = _rotation;

    switch (space)
    {
    case TS_LOCAL:
        parentSpacePoint = Transform() * point;
        _rotation = (_rotation * delta).Normalized();
        break;

    case TS_PARENT:
        parentSpacePoint = point;
        _rotation = (delta * _rotation).Normalized();
        break;

    case TS_WORLD:
        if (!parentNode)
        {
            parentSpacePoint = point;
            _rotation = (delta * _rotation).Normalized();
        }
        else
        {
            parentSpacePoint = parentNode->WorldTransform().Inverse() * point;
            Quaternion worldRotation = WorldRotation();
            _rotation = _rotation * worldRotation.Inverse() * delta * worldRotation;
        }
        break;
    }

    Vector3 oldRelativePos = oldRotation.Inverse() * (_position - parentSpacePoint);
    _position = _rotation * oldRelativePos + parentSpacePoint;

    OnTransformChanged();
}

void SpatialNode::Yaw(float angle, TransformSpace space)
{
    Rotate(Quaternion(angle, Vector3::UP), space);
}

void SpatialNode::Pitch(float angle, TransformSpace space)
{
    Rotate(Quaternion(angle, Vector3::RIGHT), space);
}

void SpatialNode::Roll(float angle, TransformSpace space)
{
    Rotate(Quaternion(angle, Vector3::FORWARD), space);
}

bool SpatialNode::LookAt(const Vector3& target, const Vector3& up, TransformSpace space)
{
    SpatialNode* parentNode = SpatialParent();
    Vector3 worldSpaceTarget;

    switch (space)
    {
    case TS_LOCAL:
        worldSpaceTarget = WorldTransform() * target;
        break;

    case TS_PARENT:
        worldSpaceTarget = !parentNode ? target : parentNode->WorldTransform() * target;
        break;

    case TS_WORLD:
        worldSpaceTarget = target;
        break;
    }

    Vector3 lookDir = worldSpaceTarget - WorldPosition();
    // Check if target is very close, in that case can not reliably calculate lookat direction
    if (lookDir.Equals(Vector3::ZERO))
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
    ApplyScale(Vector3(delta, delta, delta));
}

void SpatialNode::ApplyScale(const Vector3& delta)
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
        _worldTransform = static_cast<SpatialNode*>(Parent())->WorldTransform() * Matrix3x4(_position, _rotation, _scale);
    else
        _worldTransform = Matrix3x4(_position, _rotation, _scale);
    SetFlag(NF_WORLD_TRANSFORM_DIRTY, false);
}

}
