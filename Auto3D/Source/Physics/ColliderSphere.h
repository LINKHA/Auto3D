#pragma once
#include "Collider.h"

namespace Auto3D
{

class AUTO_API AColliderSphere : public ACollider
{
	DECLARE_CLASS(AColliderSphere, ACollider)

	DECLARE_CLASS_NEW(AColliderSphere, ACollider)
public:
	/// Construct
	AColliderSphere();
	/// Destructer
	~AColliderSphere();
	/// Register object factory.
	static void RegisterObject();
	/// Set size form float
	void SetSize(float scale);

	float GetSize() { return _size; }
private:
	/// Resize form TVector3
	void Resize(float scale);

	/// FSphere shape size (radius)
	float _size;
};

}