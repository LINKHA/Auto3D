#pragma once
#include "Collider.h"

namespace Auto3D
{

class AUTO_API AColliderSphere : public ACollider
{
	DECLARE_A_CLASS(AColliderSphere, ACollider)
public:
	/// Construct
	AColliderSphere();
	/// Destructer
	~AColliderSphere();

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