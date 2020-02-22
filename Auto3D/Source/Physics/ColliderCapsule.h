#pragma once
#include "Collider.h"

namespace Auto3D
{

class AUTO_API AColliderCapsule : public ACollider
{
	DECLARE_A_CLASS(AColliderCapsule, ACollider)
public:
	/// Construct
	AColliderCapsule();
	/// Destructer
	~AColliderCapsule();

	/// Set size radius and height
	void SetSize(float radius,float height);
	/// Set radius with this shape.
	void SetRadius(float radius);
	/// Set height with this shape.
	void SetHeight(float height);

	float GetRadius() { return _radius; }
	float GetHeight() { return _height; }
private:
	/// Resize form TVector3
	void Resize(float radius, float height);

	/// Capsule radius.
	float _radius;
	/// Capsule height.
	float _height;
};

}