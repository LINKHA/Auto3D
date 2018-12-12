#pragma once
#include "Collider.h"

namespace Auto3D {

class ColliderBox : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderBox, Collider)
public:
	explicit ColliderBox(Ambient* ambient);
	/**
	* @brief : Register object factory.
	*/
	static void RegisterObject(Ambient* ambient);

	void Start()override;

	void Update()override;

	void SetSize(const Vector3& vec);

	void SetSize(float x, float y, float z);

	void SetSize(float scale);

private:
	void resize(const Vector3& vec);

	/// box shape size
	Vector3 _size;
};

}