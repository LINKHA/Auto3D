#pragma once
#include "Collider2D.h"


namespace Auto3D {

class ColliderBox2D : public Collider2D
{
	REGISTER_DERIVED_CLASS(ColliderBox2D, Collider2D);
	DECLARE_OBJECT_SERIALIZE(ColliderBox2D);
public:
	explicit ColliderBox2D(Ambient* ambient);

};

}