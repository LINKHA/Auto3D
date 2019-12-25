#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API AColliderCircle2D : public ACollider2D
{
	DECLARE_CLASS(AColliderCircle2D, ACollider2D)
public:
	/// Construct
	AColliderCircle2D();
	/// Destructor
	virtual ~AColliderCircle2D();

	/// Set radius.
	void SetRadius(float radius);
	/// Set center.
	void SetCenter(const TVector2F& center);
	/// Set center.
	void SetCenter(float x, float y);
	/// Return radius.
	float GetRadius() const { return _radius; }
	/// Return center.
	const TVector2F& GetCenter() const { return _center; }
private:
	/// Resize form TVector2
	void RecreateFixture();

	/// Circle shape.
	b2CircleShape _circleShape;
	/// Radius.
	float _radius;
	/// Center.
	TVector2F _center;
};

}

