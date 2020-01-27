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

namespace Auto3D
{

int cmdMove(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	AWorld* world = FWorldContext::Get().GetActiveWorld();
	TVector<ACameraComponent*>& cameras = world->GetCameras();
	for (auto it = cameras.Begin(); it != cameras.End(); ++it)
	{
		ACameraComponent* camera = dynamic_cast<ACameraComponent*>(*it);

		if (_argc > 1)
		{
			if (0 == bx::strCmp(_argv[1], "forward"))
			{
				camera->SetKeyState(CAMERA_KEY_FORWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "left"))
			{
				camera->SetKeyState(CAMERA_KEY_LEFT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "right"))
			{
				camera->SetKeyState(CAMERA_KEY_RIGHT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "backward"))
			{
				camera->SetKeyState(CAMERA_KEY_BACKWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "up"))
			{
				camera->SetKeyState(CAMERA_KEY_UP, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "down"))
			{
				camera->SetKeyState(CAMERA_KEY_DOWN, true);
				return 0;
			}
		}
	}
	return 1;
}

static void cmd(const void* _userData)
{
	CmdExec((const char*)_userData);
}

static const InputBinding s_camBindings[] =
{
	{ Auto3D::Key::KeyW,             Auto3D::Modifier::None, 0, cmd, "move forward"  },
	{ Auto3D::Key::GamepadUp,        Auto3D::Modifier::None, 0, cmd, "move forward"  },
	{ Auto3D::Key::KeyA,             Auto3D::Modifier::None, 0, cmd, "move left"     },
	{ Auto3D::Key::GamepadLeft,      Auto3D::Modifier::None, 0, cmd, "move left"     },
	{ Auto3D::Key::KeyS,             Auto3D::Modifier::None, 0, cmd, "move backward" },
	{ Auto3D::Key::GamepadDown,      Auto3D::Modifier::None, 0, cmd, "move backward" },
	{ Auto3D::Key::KeyD,             Auto3D::Modifier::None, 0, cmd, "move right"    },
	{ Auto3D::Key::GamepadRight,     Auto3D::Modifier::None, 0, cmd, "move right"    },
	{ Auto3D::Key::KeyQ,             Auto3D::Modifier::None, 0, cmd, "move down"     },
	{ Auto3D::Key::GamepadShoulderL, Auto3D::Modifier::None, 0, cmd, "move down"     },
	{ Auto3D::Key::KeyE,             Auto3D::Modifier::None, 0, cmd, "move up"       },
	{ Auto3D::Key::GamepadShoulderR, Auto3D::Modifier::None, 0, cmd, "move up"       },

	INPUT_BINDING_END
};

ACameraComponent::ACameraComponent()
{
	Reset();
	Update(0.0f);

	CmdAdd("move", cmdMove);
	InputAddBindings("camBindings", s_camBindings);
}

ACameraComponent::~ACameraComponent()
{
	InputRemoveBindings("camBindings");
}

void ACameraComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ACameraComponent::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
	Update(deltaTime);
}

void ACameraComponent::Reset()
{
	_mouseNow._mx = 0;
	_mouseNow._my = 0;
	_mouseLast._mx = 0;
	_mouseLast._my = 0;
	_eye.x = 0.0f;
	_eye.y = 0.0f;
	_eye.z = -35.0f;
	_at.x = 0.0f;
	_at.y = 0.0f;
	_at.z = -1.0f;
	_up.x = 0.0f;
	_up.y = 1.0f;
	_up.z = 0.0f;
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

	const bx::Vec3 direction =
	{
		bx::cos(_verticalAngle) * bx::sin(_horizontalAngle),
		bx::sin(_verticalAngle),
		bx::cos(_verticalAngle) * bx::cos(_horizontalAngle),
	};

	const bx::Vec3 right =
	{
		bx::sin(_horizontalAngle - bx::kPiHalf),
		0,
		bx::cos(_horizontalAngle - bx::kPiHalf),
	};

	const bx::Vec3 up = bx::cross(right, direction);

	if (_keys & CAMERA_KEY_FORWARD)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(direction, deltaTime * _moveSpeed);

		_eye = bx::add(pos, tmp);
		SetKeyState(CAMERA_KEY_FORWARD, false);
	}

	if (_keys & CAMERA_KEY_BACKWARD)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(direction, deltaTime * _moveSpeed);

		_eye = bx::sub(pos, tmp);
		SetKeyState(CAMERA_KEY_BACKWARD, false);
	}

	if (_keys & CAMERA_KEY_LEFT)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(right, deltaTime * _moveSpeed);

		_eye = bx::add(pos, tmp);
		SetKeyState(CAMERA_KEY_LEFT, false);
	}

	if (_keys & CAMERA_KEY_RIGHT)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(right, deltaTime * _moveSpeed);

		_eye = bx::sub(pos, tmp);
		SetKeyState(CAMERA_KEY_RIGHT, false);
	}

	if (_keys & CAMERA_KEY_UP)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(up, deltaTime * _moveSpeed);

		_eye = bx::add(pos, tmp);
		SetKeyState(CAMERA_KEY_UP, false);
	}

	if (_keys & CAMERA_KEY_DOWN)
	{
		const bx::Vec3 pos = _eye;
		const bx::Vec3 tmp = bx::mul(up, deltaTime * _moveSpeed);

		_eye = bx::sub(pos, tmp);
		SetKeyState(CAMERA_KEY_DOWN, false);
	}

	_at = bx::add(_eye, direction);
	_up = bx::cross(right, direction);
}

void ACameraComponent::GetViewMtx(float* viewMtx)
{
	bx::mtxLookAt(viewMtx, bx::load<bx::Vec3>(&_eye.x), bx::load<bx::Vec3>(&_at.x), bx::load<bx::Vec3>(&_up.x));
}

void ACameraComponent::SetPosition(const bx::Vec3& pos)
{
	_eye = pos;
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