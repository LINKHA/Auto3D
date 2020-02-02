#pragma once
#include "Component/Controller.h"
#include "Math/Vector2.h"

namespace Auto3D
{


#define DEFAULT_CONTROLLER_KEY_FORWARD   UINT8_C(0x01)
#define DEFAULT_CONTROLLER_KEY_BACKWARD  UINT8_C(0x02)
#define DEFAULT_CONTROLLER_KEY_LEFT      UINT8_C(0x04)
#define DEFAULT_CONTROLLER_KEY_RIGHT     UINT8_C(0x08)
#define DEFAULT_CONTROLLER_KEY_UP        UINT8_C(0x10)
#define DEFAULT_CONTROLLER_KEY_DOWN      UINT8_C(0x20)

class AUTO_API ADefaultController : public AController
{
	DECLARE_A_CLASS(ADefaultController, AController)

public:
	ADefaultController();
	~ADefaultController();
	/// BeginPlay
	virtual void BeginPlay();
	/// Called every frame.
	virtual void TickComponent(float deltaTime);

	void Reset();

	void SetKeyState(uint8_t key, bool down);
private:
	float _mouseSpeed;
	float _gamepadSpeed;
	float _moveSpeed;

	uint8_t _keys;
	bool _mouseDown;

	TVector2F _mouseNow;
	TVector2F _mouseLast;

	float _horizontalAngle;
	float _verticalAngle;
};

}
