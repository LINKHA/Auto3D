#include "DefaultController.h"
#include "Gameplay/Actor.h"
#include "Component/CameraComponent.h"
#include "Platform/PlatformDef.h"
#include "Platform/cmd.h"
#include "Platform/input.h"
#include "Gameplay/WorldContext.h"
#include "Gameplay/World.h"
#include "Platform/ProcessWindow.h"
#include "Component/ControllerManager.h"
#include "Component/Transform.h"

namespace Auto3D
{

int DefaultControllerCmdMove(CmdContext* /*_context*/, void* /*_userData*/, int _argc, char const* const* _argv)
{
	GControllerManager& controllerManager = GControllerManager::Get();
	THashMap<unsigned, AController*> controllers = controllerManager.GetControllers();

	for (auto it = controllers.Begin(); it != controllers.End(); ++it)
	{
		ADefaultController* defaultController = dynamic_cast<ADefaultController*>(it->_second);
		if(!defaultController)
			continue;

		if (_argc > 1)
		{
			if (0 == bx::strCmp(_argv[1], "forward"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_FORWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "left"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_LEFT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "right"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_RIGHT, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "backward"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_BACKWARD, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "up"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_UP, true);
				return 0;
			}
			else if (0 == bx::strCmp(_argv[1], "down"))
			{
				defaultController->SetKeyState(DEFAULT_CONTROLLER_KEY_DOWN, true);
				return 0;
			}
		}
	}
	return 1;
}

static void DefaultControllerCmd(const void* _userData)
{
	CmdExec((const char*)_userData);
}

static const InputBinding defaultControllerBindings[] =
{
	{ Auto3D::Key::KeyW,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move forward"  },
	{ Auto3D::Key::GamepadUp,        Auto3D::Modifier::None, 0, DefaultControllerCmd, "move forward"  },
	{ Auto3D::Key::KeyA,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move left"     },
	{ Auto3D::Key::GamepadLeft,      Auto3D::Modifier::None, 0, DefaultControllerCmd, "move left"     },
	{ Auto3D::Key::KeyS,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move backward" },
	{ Auto3D::Key::GamepadDown,      Auto3D::Modifier::None, 0, DefaultControllerCmd, "move backward" },
	{ Auto3D::Key::KeyD,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move right"    },
	{ Auto3D::Key::GamepadRight,     Auto3D::Modifier::None, 0, DefaultControllerCmd, "move right"    },
	{ Auto3D::Key::KeyQ,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move down"     },
	{ Auto3D::Key::GamepadShoulderL, Auto3D::Modifier::None, 0, DefaultControllerCmd, "move down"     },
	{ Auto3D::Key::KeyE,             Auto3D::Modifier::None, 0, DefaultControllerCmd, "move up"       },
	{ Auto3D::Key::GamepadShoulderR, Auto3D::Modifier::None, 0, DefaultControllerCmd, "move up"       },

	INPUT_BINDING_END
};

ADefaultController::ADefaultController():
	_mouseNow({0,0}),
	_mouseLast({0,0}),
	_horizontalAngle(0.01f),
	_verticalAngle(0.0f),
	_mouseSpeed(0.0020f),
	_gamepadSpeed(0.04f),
	_moveSpeed(4.5f),
	_keys(0),
	_mouseDown(false)
{
	Reset();
	CmdAdd("move", DefaultControllerCmdMove);
	InputAddBindings("camBindings", defaultControllerBindings);
}
ADefaultController::~ADefaultController()
{
	InputRemoveBindings("camBindings");
}

void ADefaultController::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultController::TickComponent(float deltaTime)
{
	Super::TickComponent(deltaTime);
	AActor* owner = GetOwner();

	const Auto3D::MouseState& mouseState = GProcessWindow::_mouseState;
	if (!_mouseDown)
	{
		_mouseLast._x = mouseState._mx;
		_mouseLast._y = mouseState._my;
	}

	_mouseDown = !!mouseState._buttons[Auto3D::MouseButton::Right];

	if (_mouseDown)
	{
		_mouseNow._x = mouseState._mx;
		_mouseNow._y = mouseState._my;
	}

	if (_mouseDown)
	{
		int32_t deltaX = _mouseNow._x - _mouseLast._x;
		int32_t deltaY = _mouseNow._y - _mouseLast._y;

		_horizontalAngle += _mouseSpeed * float(deltaX);
		_verticalAngle -= _mouseSpeed * float(deltaY);

		_mouseLast._x = _mouseNow._x;
		_mouseLast._y = _mouseNow._y;
	}

	Auto3D::GamepadHandle handle = { 0 };
	_horizontalAngle += _gamepadSpeed * InputGetGamepadAxis(handle, Auto3D::GamepadAxis::RightX) / 32768.0f;
	_verticalAngle -= _gamepadSpeed * InputGetGamepadAxis(handle, Auto3D::GamepadAxis::RightY) / 32768.0f;
	const int32_t gpx = InputGetGamepadAxis(handle, Auto3D::GamepadAxis::LeftX);
	const int32_t gpy = InputGetGamepadAxis(handle, Auto3D::GamepadAxis::LeftY);
	_keys |= gpx < -16834 ? DEFAULT_CONTROLLER_KEY_LEFT : 0;
	_keys |= gpx > 16834 ? DEFAULT_CONTROLLER_KEY_RIGHT : 0;
	_keys |= gpy < -16834 ? DEFAULT_CONTROLLER_KEY_FORWARD : 0;
	_keys |= gpy > 16834 ? DEFAULT_CONTROLLER_KEY_BACKWARD : 0;

	
	ATransform* ownerTransform = GetOwner()->GetTransform();
	ownerTransform->SetRotation(FQuaternion(-_verticalAngle * 90, _horizontalAngle * 90, 0.0f));

	if (_keys & DEFAULT_CONTROLLER_KEY_FORWARD)
	{
		ownerTransform->Translate(TVector3F::FORWARD * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_FORWARD, false);
	}

	if (_keys & DEFAULT_CONTROLLER_KEY_BACKWARD)
	{
		ownerTransform->Translate(TVector3F::BACK * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_BACKWARD, false);
	}

	if (_keys & DEFAULT_CONTROLLER_KEY_LEFT)
	{
		ownerTransform->Translate(TVector3F::LEFT  * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_LEFT, false);
	}

	if (_keys & DEFAULT_CONTROLLER_KEY_RIGHT)
	{
		ownerTransform->Translate(TVector3F::RIGHT * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_RIGHT, false);
	}

	if (_keys & DEFAULT_CONTROLLER_KEY_UP)
	{
		ownerTransform->Translate(TVector3F::UP * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_UP, false);
	}

	if (_keys & DEFAULT_CONTROLLER_KEY_DOWN)
	{
		ownerTransform->Translate(TVector3F::DOWN * deltaTime * _moveSpeed);

		SetKeyState(DEFAULT_CONTROLLER_KEY_DOWN, false);
	}
}
void ADefaultController::Reset()
{
	_mouseNow._x = 0;
	_mouseNow._y = 0;
	_mouseLast._x = 0;
	_mouseLast._y = 0;
	_horizontalAngle = 0.01f;
	_verticalAngle = 0.0f;
	_mouseSpeed = 0.0020f;
	_gamepadSpeed = 0.04f;
	_moveSpeed = 4.5f;
	_keys = 0;
	_mouseDown = false;
}

void ADefaultController::SetKeyState(uint8_t key, bool down)
{
	_keys &= ~key;
	_keys |= down ? key : 0;
}

}