#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Platform/PlatformDef.h"
#include <bx/math.h>

#define CAMERA_KEY_FORWARD   UINT8_C(0x01)
#define CAMERA_KEY_BACKWARD  UINT8_C(0x02)
#define CAMERA_KEY_LEFT      UINT8_C(0x04)
#define CAMERA_KEY_RIGHT     UINT8_C(0x08)
#define CAMERA_KEY_UP        UINT8_C(0x10)
#define CAMERA_KEY_DOWN      UINT8_C(0x20)

namespace Auto3D
{

class AUTO_API ACameraComponent : public AActorComponent
{
	DECLARE_CLASS(ACameraComponent, AActorComponent)
public:

	struct MouseCoords
	{
		int32_t _mx;
		int32_t _my;
	};

	ACameraComponent();

	~ACameraComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	void Reset();

	void SetKeyState(uint8_t key, bool down);

	void Update(float deltaTime, const Auto3D::MouseState& mouseState);

	void GetViewMtx(float* viewMtx);

	void SetPosition(const bx::Vec3& pos);

	void SetVerticalAngle(float verticalAngle);

	void SetHorizontalAngle(float horizontalAngle);

	MouseCoords _mouseNow;
	MouseCoords _mouseLast;

	bx::Vec3 _eye;
	bx::Vec3 _at;
	bx::Vec3 _up;
	float _horizontalAngle;
	float _verticalAngle;

	float _mouseSpeed;
	float _gamepadSpeed;
	float _moveSpeed;

	uint8_t _keys;
	bool _mouseDown;
};

}