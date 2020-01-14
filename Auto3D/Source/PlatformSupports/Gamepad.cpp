#include "Gamepad.h"

namespace Auto3D
{
struct AxisDpadRemap
{
	Key::Enum first;
	Key::Enum second;
};

static AxisDpadRemap s_axisDpad[] =
{
	{ Key::GamepadLeft, Key::GamepadRight },
	{ Key::GamepadUp,   Key::GamepadDown  },
	{ Key::None,        Key::None         },
	{ Key::GamepadLeft, Key::GamepadRight },
	{ Key::GamepadUp,   Key::GamepadDown  },
	{ Key::None,        Key::None         },
};

FGamepad::FGamepad()
	: _controller(NULL)
	, _jid(INT32_MAX)
{
	bx::memSet(_value, 0, sizeof(_value));

	// Deadzone values from xinput.h
	_deadzone[GamepadAxis::LeftX] =
		_deadzone[GamepadAxis::LeftY] = 7849;
	_deadzone[GamepadAxis::RightX] =
		_deadzone[GamepadAxis::RightY] = 8689;
	_deadzone[GamepadAxis::LeftZ] =
		_deadzone[GamepadAxis::RightZ] = 30;
}

void FGamepad::Create(const SDL_JoyDeviceEvent& _jev)
{
	_joystick = SDL_JoystickOpen(_jev.which);
	SDL_Joystick* joystick = _joystick;
	_jid = SDL_JoystickInstanceID(joystick);
}

void FGamepad::Create(const SDL_ControllerDeviceEvent& _cev)
{
	_controller = SDL_GameControllerOpen(_cev.which);
	SDL_Joystick* joystick = SDL_GameControllerGetJoystick(_controller);
	_jid = SDL_JoystickInstanceID(joystick);
}

void FGamepad::Update(EventQueue& _eventQueue, WindowHandle _handle, GamepadHandle _gamepad, GamepadAxis::Enum _axis, int32_t _value)
{
	if (Filter(_axis, &_value))
	{
		_eventQueue.PostAxisEvent(_handle, _gamepad, _axis, _value);

		if (Key::None != s_axisDpad[_axis].first)
		{
			if (_value == 0)
			{
				_eventQueue.PostKeyEvent(_handle, s_axisDpad[_axis].first, 0, false);
				_eventQueue.PostKeyEvent(_handle, s_axisDpad[_axis].second, 0, false);
			}
			else
			{
				_eventQueue.PostKeyEvent(_handle
					, 0 > _value ? s_axisDpad[_axis].first : s_axisDpad[_axis].second
					, 0
					, true
				);
			}
		}
	}
}

void FGamepad::Destroy()
{
	if (NULL != _controller)
	{
		SDL_GameControllerClose(_controller);
		_controller = NULL;
	}

	if (NULL != _joystick)
	{
		SDL_JoystickClose(_joystick);
		_joystick = NULL;
	}

	_jid = INT32_MAX;
}

bool FGamepad::Filter(GamepadAxis::Enum _axis, int32_t* _value)
{
	const int32_t old = _value[_axis];
	const int32_t deadzone = _deadzone[_axis];
	int32_t value = *_value;
	value = value > deadzone || value < -deadzone ? value : 0;
	_value[_axis] = value;
	*_value = value;
	return old != value;
}

}