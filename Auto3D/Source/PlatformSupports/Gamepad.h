#pragma once
#include "AutoConfig.h"
#include "PlatformSupports/Platform.h"
#include "PlatformSupports/PlatformDef.h"

#include <SDL.h>
#include <SDL_syswm.h>

#include <stdint.h>

namespace Auto3D
{
struct FGamepad
{
	FGamepad();

	void create(const SDL_JoyDeviceEvent& _jev);

	void create(const SDL_ControllerDeviceEvent& _cev);

	void update(EventQueue& _eventQueue, WindowHandle _handle, GamepadHandle _gamepad, GamepadAxis::Enum _axis, int32_t _value);

	void destroy();

	bool filter(GamepadAxis::Enum _axis, int32_t* _value);

	int32_t m_value[GamepadAxis::Count];
	int32_t m_deadzone[GamepadAxis::Count];

	SDL_Joystick*       m_joystick;
	SDL_GameController* m_controller;
	//		SDL_Haptic*         m_haptic;
	SDL_JoystickID      m_jid;

	

};

}