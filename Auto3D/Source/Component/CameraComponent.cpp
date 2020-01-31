#include "Component/CameraComponent.h"
#include <bx/timer.h>
#include <bx/math.h>
#include "Platform/PlatformDef.h"
#include "Platform/cmd.h"
#include "Platform/input.h"
#include <bx/allocator.h>

#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"
#include "Math/Matrix4x4.h"
#include "Math/Matrix3x4.h"
#include "Component/Transform.h"

namespace Auto3D
{


ACameraComponent::ACameraComponent()
{
	Reset();
}

ACameraComponent::~ACameraComponent()
{
}

void ACameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
}

void ACameraComponent::Reset()
{
	_mouseNow._mx = 0;
	_mouseNow._my = 0;
	_mouseLast._mx = 0;
	_mouseLast._my = 0;
	_horizontalAngle = 0.01f;
	_verticalAngle = 0.0f;
	_mouseSpeed = 0.0020f;
	_gamepadSpeed = 0.04f;
	_moveSpeed = 4.5f;
	_keys = 0;
	_mouseDown = false;
}

void ACameraComponent::SetKeyState(uint8_t key, bool down)
{
	_keys &= ~key;
	_keys |= down ? key : 0;
}

void ACameraComponent::Update(float deltaTime)
{
	const Auto3D::MouseState& mouseState = GProcessWindow::_mouseState;
	if (!_mouseDown)
	{
		_mouseLast._mx = mouseState._mx;
		_mouseLast._my = mouseState._my;
	}

	_mouseDown = !!mouseState._buttons[Auto3D::MouseButton::Right];

	if (_mouseDown)
	{
		_mouseNow._mx = mouseState._mx;
		_mouseNow._my = mouseState._my;
	}

	if (_mouseDown)
	{
		int32_t deltaX = _mouseNow._mx - _mouseLast._mx;
		int32_t deltaY = _mouseNow._my - _mouseLast._my;

		_horizontalAngle += _mouseSpeed * float(deltaX);
		_verticalAngle -= _mouseSpeed * float(deltaY);

		_mouseLast._mx = _mouseNow._mx;
		_mouseLast._my = _mouseNow._my;
	}

	Auto3D::GamepadHandle handle = { 0 };
	_horizontalAngle += _gamepadSpeed * InputGetGamepadAxis(handle, Auto3D::GamepadAxis::RightX) / 32768.0f;
	_verticalAngle -= _gamepadSpeed * InputGetGamepadAxis(handle, Auto3D::GamepadAxis::RightY) / 32768.0f;
	const int32_t gpx = InputGetGamepadAxis(handle, Auto3D::GamepadAxis::LeftX);
	const int32_t gpy = InputGetGamepadAxis(handle, Auto3D::GamepadAxis::LeftY);
	_keys |= gpx < -16834 ? CAMERA_KEY_LEFT : 0;
	_keys |= gpx > 16834 ? CAMERA_KEY_RIGHT : 0;
	_keys |= gpy < -16834 ? CAMERA_KEY_FORWARD : 0;
	_keys |= gpy > 16834 ? CAMERA_KEY_BACKWARD : 0;


	ATransform* ownerTransform = GetOwner()->GetTransform();
	ownerTransform->SetRotation(FQuaternion(-_verticalAngle * 50, _horizontalAngle * 50, 0.0f));

	if (_keys & CAMERA_KEY_FORWARD)
	{
		ownerTransform->Translate(TVector3F::FORWARD * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_FORWARD, false);
	}

	if (_keys & CAMERA_KEY_BACKWARD)
	{
		ownerTransform->Translate(TVector3F::BACK * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_BACKWARD, false);
	}

	if (_keys & CAMERA_KEY_LEFT)
	{
		ownerTransform->Translate(TVector3F::LEFT  * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_LEFT, false);
	}

	if (_keys & CAMERA_KEY_RIGHT)
	{
		ownerTransform->Translate(TVector3F::RIGHT * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_RIGHT, false);
	}

	if (_keys & CAMERA_KEY_UP)
	{
		ownerTransform->Translate(TVector3F::UP * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_UP, false);
	}

	if (_keys & CAMERA_KEY_DOWN)
	{
		ownerTransform->Translate(TVector3F::DOWN * deltaTime * _moveSpeed);

		SetKeyState(CAMERA_KEY_DOWN, false);
	}
}

TMatrix3x4F ACameraComponent::EffectiveWorldTransform() const
{
	AActor* owner = GetOwner();
	ATransform* transform = owner->GetTransform();
	TMatrix3x4F worldTransform(transform->GetWorldPosition(), transform->GetWorldRotation(), 1.0f);

	return worldTransform;
}


const TMatrix3x4F& ACameraComponent::GetViewMatrix()
{
	AActor* owner = GetOwner();
	ATransform* transform = owner->GetTransform();

	if(transform->IsDirty())
		_viewMatrix = EffectiveWorldTransform().Inverse();

	return _viewMatrix;
}

void ACameraComponent::SetVerticalAngle(float verticalAngle)
{
	_verticalAngle = verticalAngle;
}

void ACameraComponent::SetHorizontalAngle(float horizontalAngle)
{
	_horizontalAngle = horizontalAngle;
}

}