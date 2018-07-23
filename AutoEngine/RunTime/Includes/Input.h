#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "stl_use.h"
#include "Singleton.h"
#include "Math/Vector2.h"
AUTO_BEGIN
USING_MATH
struct JoystickState
{
	bool GetButtonDown(unsigned index)const { return index < _buttons.size() ? _buttons[index] : false; }
	bool GetButtonPress(unsigned index)const { return index < _buttonPress.size() ? _buttonPress[index] : false; }
	_VECTOR(bool)	_buttons;
	_VECTOR(bool)	_buttonPress;
};
class Input : public Singleton<Input>
{
public:
	Input();
	~Input();

	void HandleWindowEvents();
	void Update();
	void SetKey(int key, bool newState);
	void EndFrame();

	bool GetKeyDown(int key);
	bool GetKeyPress(int key);
	Vector2& GetMousePosition() { return _mousePosition; }
	Vector2& GetMouseMove()		{ return _mouseMove; }
	bool IsMouseMove()			{ return _isMouseMove; }
private:
	void HandleSDLEvent(void* sdlEvent);
	
private:
	_SET(int)	_keysDown;
	_SET(int)	_keysPress;
	Vector2		_mousePosition;
	Vector2		_lastMousePosition;
	Vector2		_mouseMove;
	bool		_isMouseMove;
};

AUTO_END

