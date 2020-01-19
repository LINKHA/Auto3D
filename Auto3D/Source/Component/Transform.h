#pragma once

#include "Component/ActorComponent.h"
#include "Container/Vector.h"
#include "Math/Quaternion.h"
#include "Math/Matrix3x4.h"
#include "Adapter/Ptr.h"

namespace Auto3D
{


/// Transform space for translations and rotations.
namespace ETransformSpace
{
	enum Type
	{
		LOCAL = 0,
		PARENT,
		WORLD
	};
};

/// Base class for actor with position in three-dimensional space.
class AUTO_API ATransform : public AActorComponent
{
public:
    /// Construct.
    ATransform();
	~ATransform() {}

    /// Set _position in parent space.
    void SetPosition(const TVector3F& newPosition);
    /// Set rotation in parent space.
    void SetRotation(const FQuaternion& newRotation);
    /// Set forward direction in parent space.
    void SetDirection(const TVector3F& newDirection);
	/// Set scale in parent space.
	void SetScale(float newScale);
    /// Set scale in parent space.
    void SetScale(const TVector3F& newScale);
    /// Set transform in parent space.
    void SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation);
    /// Set transform in parent space.
    void SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale);
    /// Set transform in parent space.
    void SetTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale);
    /// Set _position in world space.
    void SetWorldPosition(const TVector3F& newPosition);
    /// Set rotation in world space
    void SetWorldRotation(const FQuaternion& newRotation);
    /// Set forward direction in world space.
    void SetWorldDirection(const TVector3F& newDirection);
    /// Set scale in world space.
    void SetWorldScale(const TVector3F& newScale);
    /// Set uniform scale in world space.
    void SetWorldScale(float newScale);
    /// Set transform in world space.
    void SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation);
    /// Set transform in world space.
    void SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, const TVector3F& newScale);
    /// Set transform in world space.
    void SetWorldTransform(const TVector3F& newPosition, const FQuaternion& newRotation, float newScale);
    /// Move the scene node in the chosen transform space.
    void Translate(const TVector3F& delta, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Rotate the scene node in the chosen transform space.
    void Rotate(const FQuaternion& delta, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Rotate around a point in the chosen transform space.
    void RotateAround(const TVector3F& point, const FQuaternion& delta, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Rotate around the X axis.
    void Pitch(float angle, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Rotate around the Y axis.
    void Yaw(float angle, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Rotate around the Z axis.
    void Roll(float angle, ETransformSpace::Type space = ETransformSpace::LOCAL);
    /// Look at a target _position in the chosen transform space. Note that the up vector is always specified in world space. Return true if successful, or false if resulted in an illegal rotation, in which case the current rotation remains.
    bool LookAt(const TVector3F& target, const TVector3F& up = TVector3F::UP, ETransformSpace::Type space = ETransformSpace::WORLD);
    /// Apply a scale change.
    void ApplyScale(const TVector3F& delta);
    /// Apply an uniform scale change.
    void ApplyScale(float delta);

    /// Return the parent spatial node, or null if it is not spatial.
	SPtr<ATransform> GetParentTransform() const;
    /// Return _position in parent space.
    const TVector3F& GetPosition() const { return _position; }
    /// Return rotation in parent space.
    const FQuaternion& GetRotation() const { return _rotation; }
    /// Return forward direction in parent space.
    TVector3F GetDirection() const { return _rotation * TVector3F::FORWARD; }
    /// Return scale in parent space.
    const TVector3F& GetScale() const { return _scale; }
    /// Return transform matrix in parent space.
    TMatrix3x4F GetTransform() const { return TMatrix3x4F(_position, _rotation, _scale); }
    /// Return _position in world space.
    TVector3F GetWorldPosition() const { return GetWorldTransform().Translation(); }
    /// Return rotation in world space.
    FQuaternion GetWorldRotation() const { return GetWorldTransform().Rotation(); }
    /// Return forward direction in world space.
    TVector3F GetWorldDirection() const { return GetWorldRotation() * TVector3F::FORWARD; }
    /// Return scale in world space. As it is calculated from the world transform matrix, it may not be meaningful or accurate in all cases.
    TVector3F GetWorldScale() const { return GetWorldTransform().Scale(); }
    /// Return world transform matrix.
	const TMatrix3x4F& GetWorldTransform() const;
    /// Convert a local space _position to world space.
    TVector3F GetLocalToWorld(const TVector3F& point) const { return GetWorldTransform() * point; }
    /// Convert a local space vector (either _position or direction) to world space.
    TVector3F GetLocalToWorld(const TVector4F& vector) const { return GetWorldTransform() * vector; }
    /// Convert a world space _position to local space.
    TVector3F GetWorldToLocal(const TVector3F& point) const { return GetWorldTransform().Inverse() * point; }
    /// Convert a world space vector (either _position or direction) to world space.
    TVector3F GetWorldToLocal(const TVector4F& vector) const { return GetWorldTransform().Inverse() * vector; }
private:

	void OnTransformChanged();
    /// Update world transform matrix from owner chain.
    void UpdateWorldTransform() const;
    /// World transform matrix.
    mutable TMatrix3x4F _worldTransform;
    /// Parent space _position.
    TVector3F _position;
    /// Parent space rotation.
    FQuaternion _rotation;
    /// Parent space scale.
    TVector3F _scale;

	bool _bWorldTransformDirty;
};

}
