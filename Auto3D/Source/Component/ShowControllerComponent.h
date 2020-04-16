#pragma once
#pragma once
#include "Component/ControllerComponent.h"
#include "Math/Vector2.h"

namespace Auto3D
{

class AUTO_API AShowControllerComponent : public AControllerComponent
{
	DECLARE_A_CLASS(AShowControllerComponent, AControllerComponent)

public:
	AShowControllerComponent();
	~AShowControllerComponent();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);
};

}
