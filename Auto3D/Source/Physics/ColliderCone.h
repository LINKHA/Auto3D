#pragma once
#include "Collider.h"

namespace Auto3D
{

class AUTO_API AColliderCone : public ACollider
{
	REGISTER_OBJECT_CLASS(AColliderCone, ACollider)
public:
	/// Construct
	AColliderCone();
	/// Destructer
	~AColliderCone();
	/// Register object factory.
	static void RegisterObject();

	/// Set size radius and height
	void SetSize(float radius, float height);
	/// Set radius with this shape.
	void SetRadius(float radius);
	/// Set height with this shape.
	void SetHeight(float height);

private:
	/// Resize form TVector3
	void Resize(float radius, float height);

	/// Capsule radius.
	float _radius;
	/// Capsule height.
	float _height;
};

}