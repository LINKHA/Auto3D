#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API ColliderCircle2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(ColliderCircle2D, Node2D)
public:
	/// Construct
	ColliderCircle2D();
	/// Destructor
	virtual ~ColliderCircle2D();
	/// Register object factory.
	static void RegisterObject();

	/// Set radius.
	void SetRadius(float radius);
	/// Set center.
	void SetCenter(const Vector2F& center);
	/// Set center.
	void SetCenter(float x, float y);
	/// Return radius.
	float GetRadius() const { return _radius; }
	/// Return center.
	const Vector2F& GetCenter() const { return _center; }
private:
	/// Resize form Vector2
	void Resize(const Vector2F& center, float radius);

	/// Circle shape.
	b2CircleShape _circleShape;
	/// Radius.
	float _radius;
	/// Center.
	Vector2F _center;
};

}

