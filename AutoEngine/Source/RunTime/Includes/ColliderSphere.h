#pragma once
#include "Collider.h"

namespace Auto3D {

class ColliderSphere : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderSphere, Collider)
public:
	explicit ColliderSphere(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;

	void Update()override;

	void SetSize(float size);
private:
	void resize(float size);
private:
	/// sphere shape size
	float _size;
};

}