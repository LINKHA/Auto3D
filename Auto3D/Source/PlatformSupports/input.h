#include "PlatformDef.h"

namespace Auto3D
{

typedef void(*InputBindingFn)(const void* _userData);

struct InputBinding
{
	void Set(Auto3D::Key::Enum key, uint8_t modifiers, uint8_t flags, InputBindingFn fn, const void* userData = NULL)
	{
		_key = key;
		_modifiers = modifiers;
		_flags = flags;
		_fn = fn;
		_userData = userData;
	}

	void End()
	{
		_key = Auto3D::Key::None;
		_modifiers = Auto3D::Modifier::None;
		_flags = 0;
		_fn = NULL;
		_userData = NULL;
	}

	Auto3D::Key::Enum _key;
	uint8_t _modifiers;
	uint8_t _flags;
	InputBindingFn _fn;
	const void* _userData;
};

#define INPUT_BINDING_END { Auto3D::Key::None, Auto3D::Modifier::None, 0, NULL, NULL }

///
void InputInit();

///
void InputShutdown();

///
void InputAddBindings(const char* name, const InputBinding* bindings);

///
void InputRemoveBindings(const char* name);

///
void InputProcess();

///
void InputSetKeyState(Auto3D::Key::Enum  key, uint8_t modifiers, bool down);

///
bool InputGetKeyState(Auto3D::Key::Enum key, uint8_t* modifiers = NULL);

///
uint8_t InputGetModifiersState();

/// Adds single UTF-8 encoded character into input buffer.
void InputChar(uint8_t len, const uint8_t chars[4]);

/// Returns single UTF-8 encoded character from input buffer.
const uint8_t* InputGetChar();

/// Flush internal input buffer.
void InputCharFlush();

///
void InputSetMouseResolution(uint16_t width, uint16_t height);

///
void InputSetMousePos(int32_t mx, int32_t my, int32_t mz);

///
void InputSetMouseButtonState(Auto3D::MouseButton::Enum button, uint8_t state);

///
void InputSetMouseLock(bool lock);

///
void InputGetMouse(float mouse[3]);

///
bool InputIsMouseLocked();

///
void InputSetGamepadAxis(Auto3D::GamepadHandle handle, Auto3D::GamepadAxis::Enum axis, int32_t value);

///
int32_t InputGetGamepadAxis(Auto3D::GamepadHandle handle, Auto3D::GamepadAxis::Enum axis);

}