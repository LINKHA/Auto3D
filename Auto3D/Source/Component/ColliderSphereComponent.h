#pragma once
#include "Component/ColliderComponent.h"

namespace Auto3D
{

class AUTO_API AColliderSphereComponent : public AColliderComponent
{
	DECLARE_A_CLASS(AColliderSphereComponent, AColliderComponent)
public:
	/// Construct
	AColliderSphereComponent();
	/// Destructer
	~AColliderSphereComponent();

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