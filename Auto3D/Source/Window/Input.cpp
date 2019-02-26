#include "Input.h"
#include "Window.h"

#include "../Debug/DebugNew.h"

namespace Auto3D
{

Input::Input() :
    _mouseButtons(0),
    _mouseButtonsPressed(0)
{
    RegisterSubsystem(this);
}

Input::~Input()
{
    RemoveSubsystem(this);
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
    Window* window = GetSubsystem<Window>();
    if (window)
        window->PumpMessages();
}

bool Input::IsKeyDown(unsigned keyCode) const
{
    auto it = _keyDown.Find(keyCode);
    return it != _keyDown.End() ? it->second : false;
}

bool Input::IsKeyDownRaw(unsigned rawKeyCode) const
{
    auto it = _rawKeyDown.Find(rawKeyCode);
    return it != _rawKeyDown.End() ? it->second : false;
}

bool Input::IsKeyPress(unsigned keyCode) const
{
    auto it = _keyPressed.Find(keyCode);
    return it != _keyPressed.End() ? it->second : false;
}

bool Input::IsKeyPressRaw(unsigned rawKeyCode) const
{
    auto it = _rawKeyPress.Find(rawKeyCode);
    return it != _rawKeyPress.End() ? it->second : false;
}

const Vector2I& Input::MousePosition() const
{
    Window* window = GetSubsystem<Window>();
    return window ? window->MousePosition() : Vector2I::ZERO;
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
            return it.ptr;
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

    keyEvent._keyCode = keyCode;
    keyEvent._rawKeyCode = rawKeyCode;
    keyEvent._pressed = pressed;
    keyEvent._repeat = wasDown;
    SendEvent(keyEvent);
}

void Input::OnChar(unsigned unicodeChar)
{
    charInputEvent._unicodeChar = unicodeChar;
    SendEvent(charInputEvent);
}

void Input::OnMouseMove(const Vector2I& position, const Vector2I& delta)
{
    _mouseMove += delta;

    mouseMoveEvent._position = position;
    mouseMoveEvent._delta = delta;
    SendEvent(mouseMoveEvent);
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

    mouseButtonEvent._button = button;
    mouseButtonEvent._buttons = _mouseButtons;
    mouseButtonEvent._pressed = pressed;
    mouseButtonEvent._position = MousePosition();
    SendEvent(mouseButtonEvent);
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

                    touchMoveEvent._id = it->_id;
                    touchMoveEvent._position = it->_position;
                    touchMoveEvent._delta = it->_lastDelta;
                    touchMoveEvent._pressure = it->_pressure;
                    SendEvent(touchMoveEvent);
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

            touchBeginEvent._id = newTouch._id;
            touchBeginEvent._position = newTouch._position;
            touchBeginEvent._pressure = newTouch._pressure;
            SendEvent(touchBeginEvent);
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

                touchEndEvent._id = it->_id;
                touchEndEvent._position = it->_position;
                SendEvent(touchEndEvent);
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
