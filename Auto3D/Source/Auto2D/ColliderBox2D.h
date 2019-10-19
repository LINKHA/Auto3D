#pragma once
#include "Collider2D.h"

namespace Auto3D {

class AUTO_API ColliderBox2D : public Collider2D
{
	REGISTER_OBJECT_CLASS(Collider2D, Node2D)
public:
	/// Construct
	ColliderBox2D();
	/// Destructor
	virtual ~ColliderBox2D();
	/// Register object factory.
	static void RegisterObject();
private:
	/// Box shape.
	b2PolygonShape _boxShape;
};

}

