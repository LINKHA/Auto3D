#pragma once
#include "Collider.h"

namespace Auto3D {

class ColliderBox : public Collider
{
	REGISTER_DERIVED_CLASS(ColliderBox, Collider);
	DECLARE_OBJECT_SERIALIZE(ColliderBox);
public:
	explicit ColliderBox(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

private:
	
	/// Shape size.
	Vector3 _size;
};

}