#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "stl_use.h"
AUTO_BEGIN
struct JoystickState
{
	bool GetButtonDown(unsigned index)const { return index < _buttons.size() ? _buttons[index] : false; }
	bool GetButtonPress(unsigned index)const { return index < _buttonPress.size() ? _buttonPress[index] : false; }
	_VECTOR(bool)	_buttons;
	_VECTOR(bool)	_buttonPress;
};
class Input
{
public:
	Input();
	~Input();
	/*static bool GetKeyButtonDown(int index);
	static bool GetKeyButtonRelease(int index);
	static bool GetKeyButtonRepeat(int index);

	static bool GetMouseButtonDown(int index);
	static bool GetMouseButtonRelease(int index);
	static bool GetMouseButtonRepeat(int index);*/

	
	
	//bool GetKeyDown(int key) const { return _buttons; }
	//bool GetKeyPress(int key) const {}
	_VECTOR(bool)	_buttonDown;
	_VECTOR(bool)	_buttonPress;
};

AUTO_END

