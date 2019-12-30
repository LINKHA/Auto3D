#pragma once
#include "GameFramework/Controller.h"
namespace Auto3D
{
class AUTO_API ADefaultController : public AController
{
	DECLARE_CLASS(ADefaultController, AController)
public:
	ADefaultController();
	~ADefaultController();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	float yaw;

	float pitch;
};
}
