#pragma once
#include "Collider.h"

namespace Auto3D
{

class AUTO_API AColliderSphere : public ACollider
{
	REGISTER_OBJECT(AColliderSphere, ACollider)
public:
	/// Construct
	AColliderSphere();
	/// Destructer
	~AColliderSphere();
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