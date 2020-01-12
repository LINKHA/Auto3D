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
	: m_controller(NULL)
	, m_jid(INT32_MAX)
{
	bx::memSet(m_value, 0, sizeof(m_value));

	// Deadzone values from xinput.h
	m_deadzone[GamepadAxis::LeftX] =
		m_deadzone[GamepadAxis::LeftY] = 7849;
	m_deadzone[GamepadAxis::RightX] =
		m_deadzone[GamepadAxis::RightY] = 8689;
	m_deadzone[GamepadAxis::LeftZ] =
		m_deadzone[GamepadAxis::RightZ] = 30;
}

void FGamepad::create(const SDL_JoyDeviceEvent& _jev)
{
	m_joystick = SDL_JoystickOpen(_jev.which);
	SDL_Joystick* joystick = m_joystick;
	m_jid = SDL_JoystickInstanceID(joystick);
}

void FGamepad::create(const SDL_ControllerDeviceEvent& _cev)
{
	m_controller = SDL_GameControllerOpen(_cev.which);
	SDL_Joystick* joystick = SDL_GameControllerGetJoystick(m_controller);
	m_jid = SDL_JoystickInstanceID(joystick);
}

void FGamepad::update(EventQueue& _eventQueue, WindowHandle _handle, GamepadHandle _gamepad, GamepadAxis::Enum _axis, int32_t _value)
{
	if (filter(_axis, &_value))
	{
		_eventQueue.postAxisEvent(_handle, _gamepad, _axis, _value);

		if (Key::None != s_axisDpad[_axis].first)
		{
			if (_value == 0)
			{
				_eventQueue.postKeyEvent(_handle, s_axisDpad[_axis].first, 0, false);
				_eventQueue.postKeyEvent(_handle, s_axisDpad[_axis].second, 0, false);
			}
			else
			{
				_eventQueue.postKeyEvent(_handle
					, 0 > _value ? s_axisDpad[_axis].first : s_axisDpad[_axis].second
					, 0
					, true
				);
			}
		}
	}
}

void FGamepad::destroy()
{
	if (NULL != m_controller)
	{
		SDL_GameControllerClose(m_controller);
		m_controller = NULL;
	}

	if (NULL != m_joystick)
	{
		SDL_JoystickClose(m_joystick);
		m_joystick = NULL;
	}

	m_jid = INT32_MAX;
}

bool FGamepad::filter(GamepadAxis::Enum _axis, int32_t* _value)
{
	const int32_t old = m_value[_axis];
	const int32_t deadzone = m_deadzone[_axis];
	int32_t value = *_value;
	value = value > deadzone || value < -deadzone ? value : 0;
	m_value[_axis] = value;
	*_value = value;
	return old != value;
}

}