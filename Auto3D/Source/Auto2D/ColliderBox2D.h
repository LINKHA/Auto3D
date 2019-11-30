#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API ColliderBox2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(ColliderBox2D, Collider2D)
public:
	/// Construct
	ColliderBox2D();
	/// Destructor
	virtual ~ColliderBox2D();
	/// Register object factory.
	static void RegisterObject();
	/// Set size form TVector2
	void SetSize(const TVector2F& size);
	/// Set size form float x y z
	void SetSize(float x, float y);
	/// Set size form float
	void SetSize(float scale);

private:
	/// Resize form TVector2
	void RecreateFixture();

	/// Box shape.
	b2PolygonShape _boxShape;
	/// Box shape size (radius)
	TVector2F _size;
};

}

