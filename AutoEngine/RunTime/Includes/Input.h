#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "stl_use.h"
#include "GameManager.h"
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
/**
* @brief : SubSystem class
*/
class Input : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Input, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Input);
public:
	explicit Input(Ambient* ambient);


	void HandleWindowEvents();
	void Update();
	void SetKey(int key, bool newState);
	void SetWheel(int delta);
	void EndFrame();

	bool GetKeyDown(int key);
	bool GetKeyPress(int key);
	Vector2& GetMousePosition() { return _mousePosition; }
	Vector2& GetMouseMove()		{ return _mouseMove; }
	bool IsMouseMove()			{ return _isMouseMove; }
	int GetMouseWheelMove()		{ return _mouseMoveWheel; }
	int GetMouseWheelOffset()	{ return _mouseWheelOffset; }
	void LockCursor(int x, int y);
	void LockCursorInCenter();
	void ShowCursor(bool enable);
private:
	void handleSDLEvent(void* sdlEvent);
	void lockCursor(int x, int y);
private:
	_SET(int)	_keysDown;
	_SET(int)	_keysPress;
	Vector2		_mousePosition;
	Vector2		_lastMousePosition;
	Vector2		_mouseMove;
	int			_mouseWheelOffset{};
	int			_mouseMoveWheel{};
	bool		_isMouseMove;
	bool		_isLockCursor;
	Vector2		_lockMousePosition;
};

AUTO_END

