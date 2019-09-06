#pragma once
#include "Collider.h"

namespace Auto3D
{

class ColliderCapsule : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderCapsule, Collider)
public:
	/// Construct
	ColliderCapsule();
	/// Destructer
	~ColliderCapsule();
	/// Register object factory.
	static void RegisterObject();

	/// Set size radius and height
	void SetSize(float radius,float height);
	/// Set radius with this shape.
	void SetRadius(float radius);
	/// Set height with this shape.
	void SetHeight(float height);

private:
	/// Resize form Vector3
	void Resize(float radius, float height);

	/// Capsule radius.
	float _radius;
	/// Capsule height.
	float _height;
};

}