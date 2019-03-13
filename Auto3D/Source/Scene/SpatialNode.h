#pragma once

#include "Node.h"

namespace Auto3D
{

/// Transform space for translations and rotations.
struct __TransformSpace
{
	enum _TransformSpace
	{
		LOCAL = 0,
		PARENT,
		WORLD
	};
};
using TransformSpace = __TransformSpace::_TransformSpace;


/// Base class for scene nodes with _position in three-dimensional space.
class AUTO_API SpatialNode : public Node
{
    REGISTER_OBJECT_CLASS(SpatialNode, Node)

public:
    /// Construct.
    SpatialNode();

    /// Register factory and attributes.
    static void RegisterObject();

    /// Set _position in parent space.
    void SetPosition(const Vector3F& newPosition);
    /// Set rotation in parent space.
    void SetRotation(const Quaternion& newRotation);
    /// Set forward direction in parent space.
    void SetDirection(const Vector3F& newDirection);
    /// Set scale in parent space.
    void SetScale(const Vector3F& newScale);
    /// Set uniform scale in parent space.
    void SetScale(float newScale);
    /// Set transform in parent space.
    void SetTransform(const Vector3F& newPosition, const Quaternion& newRotation);
    /// Set transform in parent space.
    void SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale);
    /// Set transform in parent space.
    void SetTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale);
    /// Set _position in world space.
    void SetWorldPosition(const Vector3F& newPosition);
    /// Set rotation in world space
    void SetWorldRotation(const Quaternion& newRotation);
    /// Set forward direction in world space.
    void SetWorldDirection(const Vector3F& newDirection);
    /// Set scale in world space.
    void SetWorldScale(const Vector3F& newScale);
    /// Set uniform scale in world space.
    void SetWorldScale(float newScale);
    /// Set transform in world space.
    void SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation);
    /// Set transform in world space.
    void SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, const Vector3F& newScale);
    /// Set transform in world space.
    void SetWorldTransform(const Vector3F& newPosition, const Quaternion& newRotation, float newScale);
    /// Move the scene node in the chosen transform space.
    void Translate(const Vector3F& delta, TransformSpace space = TransformSpace::LOCAL);
    /// Rotate the scene node in the chosen transform space.
    void Rotate(const Quaternion& delta, TransformSpace space = TransformSpace::LOCAL);
    /// Rotate around a point in the chosen transform space.
    void RotateAround(const Vector3F& point, const Quaternion& delta, TransformSpace space = TransformSpace::LOCAL);
    /// Rotate around the X axis.
    void Pitch(float angle, TransformSpace space = TransformSpace::LOCAL);
    /// Rotate around the Y axis.
    void Yaw(float angle, TransformSpace space = TransformSpace::LOCAL);
    /// Rotate around the Z axis.
    void Roll(float angle, TransformSpace space = TransformSpace::LOCAL);
    /// Look at a target _position in the chosen transform space. Note that the up vector is always specified in world space. Return true if successful, or false if resulted in an illegal rotation, in which case the current rotation remains.
    bool LookAt(const Vector3F& target, const Vector3F& up = Vector3F::UP, TransformSpace space = TransformSpace::WORLD);
    /// Apply a scale change.
    void ApplyScale(const Vector3F& delta);
    /// Apply an uniform scale change.
    void ApplyScale(float delta);

    /// Return the parent spatial node, or null if it is not spatial.
    SpatialNode* GetSpatialParent() const { return TestFlag(NF_SPATIAL_PARENT) ? static_cast<SpatialNode*>(Parent()) : nullptr; }
    /// Return _position in parent space.
    const Vector3F& GetPosition() const { return _position; }
    /// Return rotation in parent space.
    const Quaternion& GetRotation() const { return _rotation; }
    /// Return forward direction in parent space.
    Vector3F GetDirection() const { return _rotation * Vector3F::FORWARD; }
    /// Return scale in parent space.
    const Vector3F& GetScale() const { return _scale; }
    /// Return transform matrix in parent space.
    Matrix3x4F GetTransform() const { return Matrix3x4F(_position, _rotation, _scale); }
    /// Return _position in world space.
    Vector3F GetWorldPosition() const { return GetWorldTransform().Translation(); }
    /// Return rotation in world space.
    Quaternion GetWorldRotation() const { return GetWorldTransform().Rotation(); }
    /// Return forward direction in world space.
    Vector3F GetWorldDirection() const { return GetWorldRotation() * Vector3F::FORWARD; }
    /// Return scale in world space. As it is calculated from the world transform matrix, it may not be meaningful or accurate in all cases.
    Vector3F GetWorldScale() const { return GetWorldTransform().Scale(); }
    /// Return world transform matrix.
    const Matrix3x4F& GetWorldTransform() const { if (TestFlag(NF_WORLD_TRANSFORM_DIRTY)) UpdateWorldTransform(); return _worldTransform; }
    /// Convert a local space _position to world space.
    Vector3F GetLocalToWorld(const Vector3F& point) const { return GetWorldTransform() * point; }
    /// Convert a local space vector (either _position or direction) to world space.
    Vector3F GetLocalToWorld(const Vector4F& vector) const { return GetWorldTransform() * vector; }
    /// Convert a world space _position to local space.
    Vector3F GetWorldToLocal(const Vector3F& point) const { return GetWorldTransform().Inverse() * point; }
    /// Convert a world space vector (either _position or direction) to world space.
    Vector3F GetWorldToLocal(const Vector4F& vector) const { return GetWorldTransform().Inverse() * vector; }

protected:
    /// Handle being assigned to a new parent node.
    virtual void OnParentSet(Node* newParent, Node* oldParent);
    /// Handle the transform matrix changing.
    virtual void OnTransformChanged();

private:
    /// Update world transform matrix from spatial parent chain.
    void UpdateWorldTransform() const;

    /// World transform matrix.
    mutable Matrix3x4F _worldTransform;
    /// Parent space _position.
    Vector3F _position;
    /// Parent space rotation.
    Quaternion _rotation;
    /// Parent space scale.
    Vector3F _scale;
};

}
