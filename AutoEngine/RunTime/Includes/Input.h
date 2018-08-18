#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "stl_use.h"
#include "GameManager.h"
#include "Math/Vector2.h"


namespace Auto3D {

struct JoystickState
{
	bool GetButtonDown(unsigned index)const { return index < _buttons.size() ? _buttons[index] : false; }
	bool GetButtonPress(unsigned index)const { return index < _buttonPress.size() ? _buttonPress[index] : false; }
	_VECTOR(bool) _buttons;
	_VECTOR(bool) _buttonPress;
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
	/**
	* @brief : Update event
	*/
	void Update();
	/**
	* @brief : Add or delete key to key array
	* @param : If newState is true add key to Press and Down array
	*	else if newState is false earse key to Press array
	*/
	void SetKey(int key, bool newState);
	/**
	* @brief : Set wheel move and offset
	*/
	void SetWheel(int delta);
	/**
	* @brief : Clear all data
	*/
	void EndFrame();
	/**
	* @brief : Get key down (You can accept multiple buttons.)
	*/
	bool GetKeyDown(int key);
	/**
	* @brief : Get key press (You can accept multiple buttons.)
	*/
	bool GetKeyPress(int key);
	/**
	* @brief : Get mouse position absolute coordinate 
	*/
	Vector2& GetMousePosition(){ return _mousePosition; }
	/**
	* @brief : Get mouse move relative coordinate 
	*/
	Vector2& GetMouseMove(){ return _mouseMove; }
	/**
	* @brief : To determine whether the mouse is moving or not
	*/
	bool IsMouseMove(){ return _isMouseMove; }
	/**
	* @brief : Get mouse wheel move
	*/
	int GetMouseWheelMove(){ return _mouseMoveWheel; }
	/**
	* @brief : Get mouse whell offset
	*/
	int GetMouseWheelOffset(){ return _mouseWheelOffset; }
	/**
	* @brief : Lock cursor in x,y from window
	*/
	void LockCursor(int x, int y);
	/**
	* @brief : Lock cursor in window center
	*/
	void LockCursorInCenter();
	/**
	* @brief : Show cursor is or not
	*/
	void ShowCursor(bool enable);
private:
	/**
	* @brief : Handle SDL event (like mouse button cilck)
	*/
	void handleSDLEvent(void* sdlEvent);
	/**
	* @brief : Lock cursor to x,y run in event
	*/
	void lockCursorEvent(int x, int y);
private:
	_SET(int) _keysDown;
	_SET(int) _keysPress;
	Vector2	_mousePosition;
	Vector2	_lastMousePosition;
	Vector2	_mouseMove;
	int	_mouseWheelOffset{};
	int	_mouseMoveWheel{};
	bool _isMouseMove;
	bool _isLockCursor;
	Vector2	_lockMousePosition;
};

}

