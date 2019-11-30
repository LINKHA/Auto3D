#pragma once
#include "Collider.h"

namespace Auto3D
{

class ColliderSphere : public Collider
{
	REGISTER_OBJECT_CLASS(ColliderSphere, Collider)
public:
	/// Construct
	ColliderSphere();
	/// Destructer
	~ColliderSphere();
	/// Register object factory.
	static void RegisterObject();
	/// Set size form float
	void SetSize(float scale);
private:
	/// Resize form TVector3
	void Resize(float scale);

	/// FSphere shape size (radius)
	float _size;
};

}