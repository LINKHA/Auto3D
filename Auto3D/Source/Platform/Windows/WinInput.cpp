#include "WinInput.h"
#include "WinWindow.h"
#include "Core/Modules/ModuleManager.h"
#include "Graphics/Graphics.h"

#include "Debug/DebugNew.h"

namespace Auto3D
{

Input::Input() :
    _mouseButtons(0),
    _mouseButtonsPressed(0)
{
}

Input::~Input()
{
}

void Input::Update()
{
    // Clear accumulated input from last frame
    _mouseButtonsPressed = 0;
    _mouseMove = Vector2I::ZERO;
	_mouseWhellOffset = Vector2I::ZERO;
    _keyPressed.Clear();
    _rawKeyPress.Clear();
    for (auto it = _touches.Begin(); it != _touches.End(); ++it)
        it->_delta = Vector2I::ZERO;

    // The OS-specific _window message handling will call back to Input and update the state
    Window* window = ModuleManager::Get().GraphicsModule()->RenderWindow();
    if (window)
        window->PumpMessages();
}

bool Input::IsKeyDown(unsigned keyCode) const
{
    auto it = _keyDown.Find(keyCode);
    return it != _keyDown.End() ? it->_second : false;
}

bool Input::IsKeyDownRaw(unsigned rawKeyCode) const
{
    auto it = _rawKeyDown.Find(rawKeyCode);
    return it != _rawKeyDown.End() ? it->_second : false;
}

bool Input::IsKeyPress(unsigned keyCode) const
{
    auto it = _keyPressed.Find(keyCode);
    return it != _keyPressed.End() ? it->_second : false;
}

bool Input::IsKeyPressRaw(unsigned rawKeyCode) const
{
    auto it = _rawKeyPress.Find(rawKeyCode);
    return it != _rawKeyPress.End() ? it->_second : false;
}

const Vector2I& Input::GetMousePosition() const
{
	Window* window = ModuleManager::Get().GraphicsModule()->RenderWindow();
    return window ? window->GetMousePosition() : Vector2I::ZERO;
}

bool Input::IsMouseButtonDown(unsigned button) const
{
    return (_mouseButtons & (1 << button)) != 0;
}

bool Input::IsMouseButtonPress(unsigned button) const
{
    return (_mouseButtonsPressed & (1 << button)) != 0;
}

const Touch* Input::FindTouch(unsigned id) const
{
    for (auto it = _touches.Begin(); it != _touches.End(); ++it)
    {
        if (it->_id == id)
            return it._ptr;
    }

    return nullptr;
}

void Input::OnKey(unsigned keyCode, unsigned rawKeyCode, bool pressed)
{
    bool wasDown = IsKeyDown(keyCode);

    _keyDown[keyCode] = pressed;
    _rawKeyDown[rawKeyCode] = pressed;
    if (pressed)
    {
        _keyPressed[keyCode] = true;
        _rawKeyPress[rawKeyCode] = true;
    }

    _keyEvent._keyCode = keyCode;
    _keyEvent._rawKeyCode = rawKeyCode;
    _keyEvent._pressed = pressed;
    _keyEvent._repeat = wasDown;
    SendEvent(_keyEvent);
}

void Input::OnChar(unsigned unicodeChar)
{
    _charInputEvent._unicodeChar = unicodeChar;
    SendEvent(_charInputEvent);
}

void Input::OnMouseMove(const Vector2I& position, const Vector2I& delta)
{
    _mouseMove += delta;

    _mouseMoveEvent._position = position;
    _mouseMoveEvent._delta = delta;
    SendEvent(_mouseMoveEvent);
}

void Input::OnMouseWheel(const Vector2I& delta)
{
	_mouseWhellOffset = delta;
}

void Input::OnMouseButton(unsigned button, bool pressed)
{
    unsigned bit = 1 << button;

    if (pressed)
    {
        _mouseButtons |= bit;
        _mouseButtonsPressed |= bit;
    }
    else
        _mouseButtons &= ~bit;

    _mouseButtonEvent._button = button;
    _mouseButtonEvent._buttons = _mouseButtons;
    _mouseButtonEvent._pressed = pressed;
    _mouseButtonEvent._position = GetMousePosition();
    SendEvent(_mouseButtonEvent);
}

void Input::OnTouch(unsigned internalId, bool pressed, const Vector2I& position, float pressure)
{
    if (pressed)
    {
        bool found = false;

        // Ongoing touch
        for (auto it = _touches.Begin(); it != _touches.End(); ++it)
        {
            if (it->_internalId == internalId)
            {
                found = true;
                it->_lastDelta = position - it->_position;
                
                if (it->_lastDelta != Vector2I::ZERO || pressure != it->_pressure)
                {
                    it->_delta += it->_lastDelta;
                    it->_position = position;
                    it->_pressure = pressure;

                    _touchMoveEvent._id = it->_id;
                    _touchMoveEvent._position = it->_position;
                    _touchMoveEvent._delta = it->_lastDelta;
                    _touchMoveEvent._pressure = it->_pressure;
                    SendEvent(_touchMoveEvent);
                }

                break;
            }
        }

        // New touch
        if (!found)
        {
            // Use the first gap in current _touches, or insert to the end if no gaps
            size_t insertIndex = _touches.Size();
            unsigned newId = (unsigned)_touches.Size();

            for (size_t i = 0; i < _touches.Size(); ++i)
            {
                if (_touches[i]._id > i)
                {
                    insertIndex = i;
                    newId = i ? _touches[i - 1]._id + 1 : 0;
                    break;
                }
            }

            Touch newTouch;
            newTouch._id = newId;
            newTouch._internalId = internalId;
            newTouch._position = position;
            newTouch._pressure = pressure;
            _touches.Insert(insertIndex, newTouch);

            _touchBeginEvent._id = newTouch._id;
            _touchBeginEvent._position = newTouch._position;
            _touchBeginEvent._pressure = newTouch._pressure;
            SendEvent(_touchBeginEvent);
        }
    }
    else
    {
        // End touch
        for (auto it = _touches.Begin(); it != _touches.End(); ++it)
        {
            if (it->_internalId == internalId)
            {
                it->_position = position;
                it->_pressure = pressure;

                _touchEndEvent._id = it->_id;
                _touchEndEvent._position = it->_position;
                SendEvent(_touchEndEvent);
                _touches.Erase(it);
                break;
            }
        }
    }
}

void Input::OnGainFocus()
{
}

void Input::OnLoseFocus()
{
    _mouseButtons = 0;
    _mouseButtonsPressed = 0;
    _mouseMove = Vector2I::ZERO;
    _keyDown.Clear();
    _keyPressed.Clear();
    _rawKeyDown.Clear();
    _rawKeyPress.Clear();
}

}
