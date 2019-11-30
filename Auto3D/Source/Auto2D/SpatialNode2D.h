#pragma once

#include "Math/Matrix3x4.h"
#include "Node2D.h"

namespace Auto3D
{

/// Transform space for translations and rotations.
namespace Transform2DSpace
{
	enum Type
	{
		LOCAL = 0,
		PARENT,
		WORLD
	};
};


/// Base class for scene nodes with _position in three-dimensional space.
class AUTO_API SpatialNode2D : public Node2D
{
	REGISTER_OBJECT_CLASS(SpatialNode2D, Node2D)

public:
	/// Construct.
	SpatialNode2D();
	/// The destructor
	~SpatialNode2D() = default;
	/// Register factory and attributes.
	static void RegisterObject();

	/// Set position in parent space.
	void SetPosition(const TVector3F& newPosition);
	/// Set position in parent space 2D.
	void SetPosition2D(const TVector2F& position) { SetPosition(TVector3F(position)); }
	/// Set rotation in parent space.
	void SetRotation(const FQuaternion& newRotation);
	/// Set rotation in parent space 2D.
	void SetRotation2D(float rotation) { SetRotation(FQuaternion(rotation)); }
	/// Set forward direction in parent space.
	void SetDirection(const TVector3F& newDirection);
	/// Set scale in parent space.
	void SetScale(const TVector3F& newScale);
	/// Set uniform scale in parent space.
	void SetScale(float newScale);
	/// Set scale in parent space 2D.
	void SetScale2D(const TVector2F& scale) { SetScale(TVector3F(scale, 1.0f)); }
	/// Set scale in parent space 2D.
	void SetScale2D(float x, float y) { SetScale(TVector3F(x, y, 1.0f)); }

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
	void Translate(const TVector3F& delta, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Rotate the scene node in the chosen transform space.
	void Rotate(const FQuaternion& delta, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Rotate around a point in the chosen transform space.
	void RotateAround(const TVector3F& point, const FQuaternion& delta, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Rotate around the X axis.
	void Pitch(float angle, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Rotate around the Y axis.
	void Yaw(float angle, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Rotate around the Z axis.
	void Roll(float angle, Transform2DSpace::Type space = Transform2DSpace::LOCAL);
	/// Look at a target _position in the chosen transform space. Note that the up vector is always specified in world space. Return true if successful, or false if resulted in an illegal rotation, in which case the current rotation remains.
	bool LookAt(const TVector3F& target, const TVector3F& up = TVector3F::UP, Transform2DSpace::Type space = Transform2DSpace::WORLD);
	/// Apply a scale change.
	void ApplyScale(const TVector3F& delta);
	/// Apply an uniform scale change.
	void ApplyScale(float delta);

	/// Return the parent spatial node, or null if it is not spatial.
	SpatialNode2D* GetSpatialParent() const { return TestFlag(NF_2D_SPATIAL_PARENT) ? static_cast<SpatialNode2D*>(Parent()) : nullptr; }
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
	const TMatrix3x4F& GetWorldTransform() const { if (TestFlag(NF_2D_WORLD_TRANSFORM_DIRTY)) UpdateWorldTransform(); return _worldTransform; }
	/// Convert a local space _position to world space.
	TVector3F GetLocalToWorld(const TVector3F& point) const { return GetWorldTransform() * point; }
	/// Convert a local space vector (either _position or direction) to world space.
	TVector3F GetLocalToWorld(const TVector4F& vector) const { return GetWorldTransform() * vector; }
	/// Convert a world space _position to local space.
	TVector3F GetWorldToLocal(const TVector3F& point) const { return GetWorldTransform().Inverse() * point; }
	/// Convert a world space vector (either _position or direction) to world space.
	TVector3F GetWorldToLocal(const TVector4F& vector) const { return GetWorldTransform().Inverse() * vector; }

protected:
	/// Handle being assigned to a new parent node.
	virtual void OnParentSet(Node2D* newParent, Node2D* oldParent);
	/// Handle the transform matrix changing.
	virtual void OnTransformChanged();

private:
	/// Update world transform matrix from spatial parent chain.
	void UpdateWorldTransform() const;

	/// World transform matrix.
	mutable TMatrix3x4F _worldTransform;
	/// Parent space _position.
	TVector3F _position;
	/// Parent space rotation.
	FQuaternion _rotation;
	/// Parent space scale.
	TVector3F _scale;
};

}
