#pragma once
#include "Auto.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Math/Vector2.h"


namespace Auto3D {

struct JoystickState
{
	bool GetButtonDown(unsigned index)const { return index < _buttons.size() ? _buttons[index] : false; }
	bool GetButtonPress(unsigned index)const { return index < _buttonPress.size() ? _buttonPress[index] : false; }
	VECTOR<bool> _buttons;
	VECTOR<bool> _buttonPress;
};
/**
* SubSystem class
*/
class Input : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Input, GlobalGameManager)

	using Keys = SET<int>;
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
	Vector2 GetMousePosition(){ return _mousePosition; }
	/**
	* @brief : Get mouse move relative coordinate 
	*/
	Vector2 GetMouseMove(){ return _mouseMove; }
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
	/**
	* @brief : Limit the mouse in the window
	*/
	void HideMouseInWindow(bool enable);
private:
	/**
	* @brief : Handle SDL event (like mouse button cilck)
	*/
	void handleSDLEvent(void* sdlEvent);
	/**
	* @brief : Lock cursor to x,y run in event
	*/
	void lockCursorEvent();
	/**
	* @brief : Limit cursor in window
	*/
	void hideCursorEvent(bool enable);
private:
	///all buttons down in the same frame
	Keys _keysDown;
	///all buttons pressed in the same frame
	Keys _keysPress;
	///mouse position limit in window
	Vector2	_mousePosition;
	///last mouse potition limit in window
	Vector2	_lastMousePosition;
	///mouse offset
	Vector2	_mouseMove;
	///mouse wheel offset
	int	_mouseWheelOffset{};
	///mouse whell move
	int	_mouseMoveWheel{};
	///is mouse move
	bool _isMouseMove;
	///is lock cursor in position
	bool _isLockCursor;
	///is hide cursor in window and you can get an offset beyond the window
	bool _isHideCursor;
	///is show cursor
	bool _isShowCursor;
	///lock mouse in position
	Vector2	_lockMousePosition;
};

}

