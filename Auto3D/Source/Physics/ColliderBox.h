#pragma once
#include "Collider.h"

namespace Auto3D 
{

class ColliderBox : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderBox, Collider)
public:
	/// Construct
	ColliderBox();
	/// Destructer
	~ColliderBox();
	/// Register object factory.
	static void RegisterObject();
	/// Set size form Vector3
	void SetSize(const Vector3F& vec);
	/// Set size form float x y z
	void SetSize(float x, float y, float z);
	/// Set size form float
	void SetSize(float scale);
private:
	/// Resize form Vector3
	void Resize(const Vector3F& vec);

	/// Box shape size
	Vector3F _size;
};

}