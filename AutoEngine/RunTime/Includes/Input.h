#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "stl_use.h"
#include "Singleton.h"
AUTO_BEGIN
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
private:
	void HandleSDLEvent(void* sdlEvent);
	
private:
	_SET(int)	_keysDown;
	_SET(int)	_keysPress;
};

AUTO_END

