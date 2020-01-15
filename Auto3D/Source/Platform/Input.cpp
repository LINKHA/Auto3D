/*
 * Copyright 2010-2019 Branimir Karadzic. All rights reserved.
 * License: https://github.com/bkaradzic/bgfx#license-bsd-2-clause
 */

#include <memory.h>

#include "PlatformDef.h"
#include "input.h"
#include "cmd.h"

#include <bx/allocator.h>
#include <bx/ringbuffer.h>
#include <tinystl/string.h>
#include <tinystl/allocator.h>
#include <tinystl/unordered_map.h>

#include "Platform/PlatformDef.h"
#include "Platform/PlatformContext.h"

namespace stl = tinystl;

namespace Auto3D
{

struct InputMouse
{
	InputMouse()
		: _width(1280)
		, _height(720)
		, _wheelDelta(120)
		, _lock(false)
	{
	}

	void Reset()
	{
		if (_lock)
		{
			_norm[0] = 0.0f;
			_norm[1] = 0.0f;
			_norm[2] = 0.0f;
		}

		bx::memSet(_buttons, 0, sizeof(_buttons));
	}

	void SetResolution(uint16_t width, uint16_t height)
	{
		_width = width;
		_height = height;
	}

	void SetPos(int32_t mx, int32_t my, int32_t mz)
	{
		_absolute[0] = mx;
		_absolute[1] = my;
		_absolute[2] = mz;
		_norm[0] = float(mx) / float(_width);
		_norm[1] = float(my) / float(_height);
		_norm[2] = float(mz) / float(_wheelDelta);
	}

	void SetButtonState(Auto3D::MouseButton::Enum button, uint8_t state)
	{
		_buttons[button] = state;
	}

	int32_t _absolute[3];
	float _norm[3];
	int32_t _wheel;
	uint8_t _buttons[Auto3D::MouseButton::Count];
	uint16_t _width;
	uint16_t _height;
	uint16_t _wheelDelta;
	bool _lock;
};

struct InputKeyboard
{
	InputKeyboard()
		: _ring(BX_COUNTOF(m_char) - 4)
	{
	}

	void Reset()
	{
		bx::memSet(m_key, 0, sizeof(m_key));
		bx::memSet(m_once, 0xff, sizeof(m_once));
	}

	static uint32_t EncodeKeyState(uint8_t _modifiers, bool _down)
	{
		uint32_t state = 0;
		state |= uint32_t(_down ? _modifiers : 0) << 16;
		state |= uint32_t(_down) << 8;
		return state;
	}

	static bool DecodeKeyState(uint32_t _state, uint8_t& _modifiers)
	{
		_modifiers = (_state >> 16) & 0xff;
		return 0 != ((_state >> 8) & 0xff);
	}

	void setKeyState(Auto3D::Key::Enum _key, uint8_t _modifiers, bool _down)
	{
		m_key[_key] = EncodeKeyState(_modifiers, _down);
		m_once[_key] = false;
	}

	bool getKeyState(Auto3D::Key::Enum _key, uint8_t* _modifiers)
	{
		uint8_t modifiers;
		_modifiers = NULL == _modifiers ? &modifiers : _modifiers;

		return DecodeKeyState(m_key[_key], *_modifiers);
	}

	uint8_t getModifiersState()
	{
		uint8_t modifiers = 0;
		for (uint32_t ii = 0; ii < Auto3D::Key::Count; ++ii)
		{
			modifiers |= (m_key[ii] >> 16) & 0xff;
		}
		return modifiers;
	}

	void pushChar(uint8_t _len, const uint8_t _char[4])
	{
		for (uint32_t len = _ring.reserve(4)
			; len < _len
			; len = _ring.reserve(4)
			)
		{
			popChar();
		}

		bx::memCopy(&m_char[_ring.m_current], _char, 4);
		_ring.commit(4);
	}

	const uint8_t* popChar()
	{
		if (0 < _ring.available())
		{
			uint8_t* utf8 = &m_char[_ring.m_read];
			_ring.consume(4);
			return utf8;
		}

		return NULL;
	}

	void charFlush()
	{
		_ring.m_current = 0;
		_ring.m_write = 0;
		_ring.m_read = 0;
	}

	uint32_t m_key[256];
	bool m_once[256];

	bx::RingBufferControl _ring;
	uint8_t m_char[256];
};

struct Gamepad
{
	Gamepad()
	{
		reset();
	}

	void reset()
	{
		bx::memSet(m_axis, 0, sizeof(m_axis));
	}

	void setAxis(Auto3D::GamepadAxis::Enum _axis, int32_t _value)
	{
		m_axis[_axis] = _value;
	}

	int32_t getAxis(Auto3D::GamepadAxis::Enum _axis)
	{
		return m_axis[_axis];
	}

	int32_t m_axis[Auto3D::GamepadAxis::Count];
};

struct Input
{
	Input()
	{
		reset();
	}

	~Input()
	{
	}

	void addBindings(const char* _name, const InputBinding* _bindings)
	{
		m_inputBindingsMap.insert(stl::make_pair(stl::string(_name), _bindings));
	}

	void removeBindings(const char* _name)
	{
		InputBindingMap::iterator it = m_inputBindingsMap.find(stl::string(_name));
		if (it != m_inputBindingsMap.end())
		{
			m_inputBindingsMap.erase(it);
		}
	}

	void process(const InputBinding* _bindings)
	{
		for (const InputBinding* binding = _bindings; binding->_key != Auto3D::Key::None; ++binding)
		{
			uint8_t modifiers;
			bool down = InputKeyboard::DecodeKeyState(m_keyboard.m_key[binding->_key], modifiers);

			if (binding->_flags == 1)
			{
				if (down)
				{
					if (modifiers == binding->_modifiers
						&& !m_keyboard.m_once[binding->_key])
					{
						if (NULL == binding->_fn)
						{
							CmdExec((const char*)binding->_userData);
						}
						else
						{
							binding->_fn(binding->_userData);
						}
						m_keyboard.m_once[binding->_key] = true;
					}
				}
				else
				{
					m_keyboard.m_once[binding->_key] = false;
				}
			}
			else
			{
				if (down
					&&  modifiers == binding->_modifiers)
				{
					if (NULL == binding->_fn)
					{
						CmdExec((const char*)binding->_userData);
					}
					else
					{
						binding->_fn(binding->_userData);
					}
				}
			}
		}
	}

	void process()
	{
		for (InputBindingMap::const_iterator it = m_inputBindingsMap.begin(); it != m_inputBindingsMap.end(); ++it)
		{
			process(it->second);
		}
	}

	void reset()
	{
		m_mouse.Reset();
		m_keyboard.Reset();
		for (uint32_t ii = 0; ii < BX_COUNTOF(m_gamepad); ++ii)
		{
			m_gamepad[ii].reset();
		}
	}

	typedef stl::unordered_map<stl::string, const InputBinding*> InputBindingMap;
	InputBindingMap m_inputBindingsMap;
	InputKeyboard m_keyboard;
	InputMouse m_mouse;
	Gamepad m_gamepad[ENTRY_CONFIG_MAX_GAMEPADS];
};

static Input* s_input;

void InputInit()
{
	s_input = BX_NEW(Auto3D::getAllocator(), Input);
}

void InputShutdown()
{
	BX_DELETE(Auto3D::getAllocator(), s_input);
}

void InputAddBindings(const char* _name, const InputBinding* _bindings)
{
	s_input->addBindings(_name, _bindings);
}

void InputRemoveBindings(const char* _name)
{
	s_input->removeBindings(_name);
}

void InputProcess()
{
	s_input->process();
}

void InputSetMouseResolution(uint16_t _width, uint16_t _height)
{
	s_input->m_mouse.SetResolution(_width, _height);
}

void InputSetKeyState(Auto3D::Key::Enum _key, uint8_t _modifiers, bool _down)
{
	s_input->m_keyboard.setKeyState(_key, _modifiers, _down);
}

bool InputGetKeyState(Auto3D::Key::Enum _key, uint8_t* _modifiers)
{
	return s_input->m_keyboard.getKeyState(_key, _modifiers);
}

uint8_t InputGetModifiersState()
{
	return s_input->m_keyboard.getModifiersState();
}

void InputChar(uint8_t len, const uint8_t chars[4])
{
	s_input->m_keyboard.pushChar(len, chars);
}

const uint8_t* InputGetChar()
{
	return s_input->m_keyboard.popChar();
}

void InputCharFlush()
{
	s_input->m_keyboard.charFlush();
}

void InputSetMousePos(int32_t _mx, int32_t _my, int32_t _mz)
{
	s_input->m_mouse.SetPos(_mx, _my, _mz);
}

void InputSetMouseButtonState(Auto3D::MouseButton::Enum _button, uint8_t _state)
{
	s_input->m_mouse.SetButtonState(_button, _state);
}

void InputGetMouse(float _mouse[3])
{
	_mouse[0] = s_input->m_mouse._norm[0];
	_mouse[1] = s_input->m_mouse._norm[1];
	_mouse[2] = s_input->m_mouse._norm[2];
	s_input->m_mouse._norm[0] = 0.0f;
	s_input->m_mouse._norm[1] = 0.0f;
	s_input->m_mouse._norm[2] = 0.0f;
}

bool InputIsMouseLocked()
{
	return s_input->m_mouse._lock;
}

void InputSetMouseLock(bool _lock)
{
	if (s_input->m_mouse._lock != _lock)
	{
		s_input->m_mouse._lock = _lock;
		Auto3D::WindowHandle defaultWindow = { 0 };
		FPlatform::SetMouseLock(PlatfromContext::_defaultWindow, _lock);

		if (_lock)
		{
			s_input->m_mouse._norm[0] = 0.0f;
			s_input->m_mouse._norm[1] = 0.0f;
			s_input->m_mouse._norm[2] = 0.0f;
		}
	}
}

void InputSetGamepadAxis(Auto3D::GamepadHandle _handle, Auto3D::GamepadAxis::Enum _axis, int32_t _value)
{
	s_input->m_gamepad[_handle.idx].setAxis(_axis, _value);
}

int32_t InputGetGamepadAxis(Auto3D::GamepadHandle _handle, Auto3D::GamepadAxis::Enum _axis)
{
	return s_input->m_gamepad[_handle.idx].getAxis(_axis);
}
}