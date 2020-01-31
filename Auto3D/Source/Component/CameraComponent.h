#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Platform/PlatformDef.h"
#include "Math/Vector3.h"
#include "Math/Matrix3x4.h"


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
	ACameraComponent();

	~ACameraComponent();

	/// BeginPlay
	virtual void BeginPlay() override;
	/// Called every frame.
	virtual void TickComponent(float deltaTime) override;

	void Reset();

	void SetKeyState(uint8_t key, bool down);

	/// Get effective world transform for matrix and frustum calculations including reflection but excluding node scaling.
	TMatrix3x4F EffectiveWorldTransform() const;

	const TMatrix3x4F& GetViewMatrix();

	void SetVerticalAngle(float verticalAngle);

	void SetHorizontalAngle(float horizontalAngle);

	void Update(float deltaTime);
private:
	float _mouseSpeed;
	float _gamepadSpeed;
	float _moveSpeed;

	uint8_t _keys;
	bool _mouseDown;

	struct MouseCoords
	{
		int32_t _mx;
		int32_t _my;
	};

	MouseCoords _mouseNow;
	MouseCoords _mouseLast;

	float _horizontalAngle;
	float _verticalAngle;

	/// Cached view matrix.
	mutable TMatrix3x4F _viewMatrix;
};

}