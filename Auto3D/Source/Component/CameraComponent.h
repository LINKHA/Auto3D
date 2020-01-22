#pragma once
#include "AutoConfig.h"
#include "Component/ActorComponent.h"
#include "Platform/PlatformDef.h"
#include <bx/math.h>

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

};


#define CAMERA_KEY_FORWARD   UINT8_C(0x01)
#define CAMERA_KEY_BACKWARD  UINT8_C(0x02)
#define CAMERA_KEY_LEFT      UINT8_C(0x04)
#define CAMERA_KEY_RIGHT     UINT8_C(0x08)
#define CAMERA_KEY_UP        UINT8_C(0x10)
#define CAMERA_KEY_DOWN      UINT8_C(0x20)
}


using namespace Auto3D;


struct Camera
{
	struct MouseCoords
	{
		int32_t m_mx;
		int32_t m_my;
	};

	Camera();

	~Camera();

	void reset();

	void setKeyState(uint8_t _key, bool _down);

	void update(float _deltaTime, const Auto3D::MouseState& _mouseState);

	void getViewMtx(float* _viewMtx);

	void setPosition(const bx::Vec3& _pos);

	void setVerticalAngle(float _verticalAngle);

	void setHorizontalAngle(float _horizontalAngle);

	MouseCoords m_mouseNow;
	MouseCoords m_mouseLast;

	bx::Vec3 m_eye;
	bx::Vec3 m_at;
	bx::Vec3 m_up;
	float m_horizontalAngle;
	float m_verticalAngle;

	float m_mouseSpeed;
	float m_gamepadSpeed;
	float m_moveSpeed;

	uint8_t m_keys;
	bool m_mouseDown;
};

///
void cameraCreate();

///
void cameraDestroy();

///
void cameraSetPosition(const bx::Vec3& _pos);

///
void cameraSetHorizontalAngle(float _horizontalAngle);

///
void cameraSetVerticalAngle(float _verticalAngle);

///
void cameraSetKeyState(uint8_t _key, bool _down);

///
void cameraGetViewMtx(float* _viewMtx);

///
bx::Vec3 cameraGetPosition();

///
bx::Vec3 cameraGetAt();

///
void cameraUpdate(float _deltaTime, const Auto3D::MouseState& _mouseState);
