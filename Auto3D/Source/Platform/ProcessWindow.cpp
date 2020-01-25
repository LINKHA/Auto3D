#include "Platform/ProcessWindow.h"
#include "Platform/Input.h"
#include "Platform/Dbg.h"
#include "Platform/Cmd.h"

#include <bgfx/bgfx.h>
#include <bx/bx.h>

#include <corecrt.h>
#include <time.h>

namespace Auto3D
{
uint32_t FProcessWindow::_debug = BGFX_DEBUG_NONE;
uint32_t FProcessWindow::_reset = BGFX_RESET_NONE;
bool FProcessWindow::_exit = false;
WindowState FProcessWindow::_window[ENTRY_CONFIG_MAX_WINDOWS];

bool setOrToggle(uint32_t& _flags, const char* _name, uint32_t _bit, int _first, int _argc, char const* const* _argv)
{
	if (0 == bx::strCmp(_argv[_first], _name))
	{
		int arg = _first + 1;
		if (_argc > arg)
		{
			_flags &= ~_bit;

			bool set = false;
			bx::fromString(&set, _argv[arg]);

			_flags |= set ? _bit : 0;
		}
		else
		{
			_flags ^= _bit;
		}

		return true;
	}

	return false;
}

int cmdGraphics(CmdContext* /*context*/, void* /*userData*/, int _argc, char const* const* _argv)
{
	if (_argc > 1)
	{
		if (setOrToggle(FProcessWindow::_reset, "vsync", BGFX_RESET_VSYNC, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "maxaniso", BGFX_RESET_MAXANISOTROPY, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "msaa", BGFX_RESET_MSAA_X16, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "flush", BGFX_RESET_FLUSH_AFTER_RENDER, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "flip", BGFX_RESET_FLIP_AFTER_RENDER, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "hidpi", BGFX_RESET_HIDPI, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_reset, "depthclamp", BGFX_RESET_DEPTH_CLAMP, 1, _argc, _argv)
			)
		{
			return bx::kExitSuccess;
		}
		else if (setOrToggle(FProcessWindow::_debug, "stats", BGFX_DEBUG_STATS, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_debug, "ifh", BGFX_DEBUG_IFH, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_debug, "text", BGFX_DEBUG_TEXT, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_debug, "wireframe", BGFX_DEBUG_WIREFRAME, 1, _argc, _argv)
			|| setOrToggle(FProcessWindow::_debug, "profiler", BGFX_DEBUG_PROFILER, 1, _argc, _argv)
			)
		{
			bgfx::setDebug(FProcessWindow::_debug);
			return bx::kExitSuccess;
		}
		else if (0 == bx::strCmp(_argv[1], "screenshot"))
		{
			bgfx::FrameBufferHandle fbh = BGFX_INVALID_HANDLE;

			if (_argc > 2)
			{
				bgfx::requestScreenShot(fbh, _argv[2]);
			}
			else
			{
				time_t tt;
				time(&tt);

				char filePath[256];
				bx::snprintf(filePath, sizeof(filePath), "temp/screenshot-%d", tt);
				bgfx::requestScreenShot(fbh, filePath);
			}

			return bx::kExitSuccess;
		}
		else if (0 == bx::strCmp(_argv[1], "fullscreen"))
		{
			WindowHandle window = { 0 };
			FPlatform::ToggleFullscreen(window);
			return bx::kExitSuccess;
		}
	}

	return bx::kExitFailure;
}

int cmdExit(CmdContext* /*context*/, void* /*userData*/, int _argc, char const* const* _argv)
{
	FProcessWindow::_exit = true;
	return bx::kExitSuccess;
}

bool ProcessWindowEvents(WindowState& state, uint32_t& debug, uint32_t& reset)
{
	FProcessWindow::_debug = debug;
	FProcessWindow::_reset = reset;

	WindowHandle handle = { UINT16_MAX };

	bool mouseLock = InputIsMouseLocked();
	bool clearDropFile = true;

	const Event* ev;
	do
	{
		struct SE
		{
			SE(WindowHandle _handle)
				: m_ev(Poll(_handle))
			{
			}

			~SE()
			{
				if (NULL != m_ev)
				{
					Release(m_ev);
				}
			}

			const Event* m_ev;

		} scopeEvent(handle);
		ev = scopeEvent.m_ev;

		if (NULL != ev)
		{
			handle = ev->_handle;
			WindowState& win = FProcessWindow::_window[handle.idx];

			switch (ev->_type)
			{
			case Event::Axis:
			{
				const AxisEvent* axis = static_cast<const AxisEvent*>(ev);
				InputSetGamepadAxis(axis->_gamepad, axis->_axis, axis->_value);
			}
			break;

			case Event::Char:
			{
				const CharEvent* chev = static_cast<const CharEvent*>(ev);
				win._handle = chev->_handle;
				InputChar(chev->_len, chev->_char);
			}
			break;

			case Event::Exit:
				return true;

			case Event::Gamepad:
			{
				const GamepadEvent* gev = static_cast<const GamepadEvent*>(ev);
				DBG("gamepad %d, %d", gev->_gamepad.idx, gev->_connected);
			}
			break;

			case Event::Mouse:
			{
				const MouseEvent* mouse = static_cast<const MouseEvent*>(ev);
				win._handle = mouse->_handle;

				if (mouse->_move)
				{
					InputSetMousePos(mouse->_mx, mouse->_my, mouse->_mz);
				}
				else
				{
					InputSetMouseButtonState(mouse->_button, mouse->_down);
				}

				if (!mouseLock)
				{
					if (mouse->_move)
					{
						win._mouse._mx = mouse->_mx;
						win._mouse._my = mouse->_my;
						win._mouse._mz = mouse->_mz;
					}
					else
					{
						win._mouse._buttons[mouse->_button] = mouse->_down;
					}
				}
			}
			break;

			case Event::Key:
			{
				const KeyEvent* key = static_cast<const KeyEvent*>(ev);
				win._handle = key->_handle;

				InputSetKeyState(key->_key, key->_modifiers, key->_down);
			}
			break;

			case Event::Size:
			{
				const SizeEvent* size = static_cast<const SizeEvent*>(ev);
				win._handle = size->_handle;
				win._width = size->_width;
				win._height = size->_height;
				reset = win._handle.idx == 0
					? !FProcessWindow::_reset
					: reset
					; // force reset
			}
			break;

			case Event::Window:
			{
				const WindowEvent* window = static_cast<const WindowEvent*>(ev);
				win._handle = window->_handle;
				win._nwh = window->_nwh;
				ev = NULL;
			}
			break;

			case Event::Suspend:
				break;

			case Event::DropFile:
			{
				const DropFileEvent* drop = static_cast<const DropFileEvent*>(ev);
				win._dropFile = drop->_filePath;
				clearDropFile = false;
			}
			break;

			default:
				break;
			}
		}

		InputProcess();

	} while (NULL != ev);

	if (isValid(handle))
	{
		WindowState& win = FProcessWindow::_window[handle.idx];
		if (clearDropFile)
		{
			win._dropFile.clear();
		}

		state = win;

		if (handle.idx == 0)
		{
			InputSetMouseResolution(uint16_t(win._width), uint16_t(win._height));
		}
	}

	if (reset != FProcessWindow::_reset)
	{
		reset = FProcessWindow::_reset;
		bgfx::reset(FProcessWindow::_window[0]._width, FProcessWindow::_window[0]._height, reset);
		InputSetMouseResolution(uint16_t(FProcessWindow::_window[0]._width), uint16_t(FProcessWindow::_window[0]._height));
	}

	debug = FProcessWindow::_debug;

	return FProcessWindow::_exit;
}

bool ProcessEvents(uint32_t& width, uint32_t& height, uint32_t& debug, uint32_t& reset, MouseState* mouse)
{
	FProcessWindow::_debug = debug;
	FProcessWindow::_reset = reset;

	WindowHandle handle = { UINT16_MAX };

	bool mouseLock = InputIsMouseLocked();

	const Event* ev;
	do
	{
		struct SE { const Event* m_ev; SE() : m_ev(Poll()) {} ~SE() { if (NULL != m_ev) { Release(m_ev); } } } scopeEvent;
		ev = scopeEvent.m_ev;

		if (NULL != ev)
		{
			switch (ev->_type)
			{
			case Event::Axis:
			{
				const AxisEvent* axis = static_cast<const AxisEvent*>(ev);
				InputSetGamepadAxis(axis->_gamepad, axis->_axis, axis->_value);
			}
			break;

			case Event::Char:
			{
				const CharEvent* chev = static_cast<const CharEvent*>(ev);
				InputChar(chev->_len, chev->_char);
			}
			break;

			case Event::Exit:
				return true;

			case Event::Gamepad:
			{
				//						const GamepadEvent* gev = static_cast<const GamepadEvent*>(ev);
				//						DBG("gamepad %d, %d", gev->m_gamepad.idx, gev->m_connected);
			}
			break;

			case Event::Mouse:
			{
				const MouseEvent* mouseEvent = static_cast<const MouseEvent*>(ev);
				handle = mouseEvent->_handle;

				InputSetMousePos(mouseEvent->_mx, mouseEvent->_my, mouseEvent->_mz);
				if (!mouseEvent->_move)
				{
					InputSetMouseButtonState(mouseEvent->_button, mouseEvent->_down);
				}

				if (NULL != mouseEvent
					&& !mouseLock)
				{
					mouse->_mx = mouseEvent->_mx;
					mouse->_my = mouseEvent->_my;
					mouse->_mz = mouseEvent->_mz;
					if (!mouseEvent->_move)
					{
						mouse->_buttons[mouseEvent->_button] = mouseEvent->_down;
					}
				}
			}
			break;

			case Event::Key:
			{
				const KeyEvent* key = static_cast<const KeyEvent*>(ev);
				handle = key->_handle;

				InputSetKeyState(key->_key, key->_modifiers, key->_down);
			}
			break;

			case Event::Size:
			{
				const SizeEvent* size = static_cast<const SizeEvent*>(ev);
				WindowState& win = FProcessWindow::_window[0];
				win._handle = size->_handle;
				win._width = size->_width;
				win._height = size->_height;

				handle = size->_handle;
				width = size->_width;
				height = size->_height;
				reset = !FProcessWindow::_reset; // force reset
			}
			break;

			case Event::Window:
				break;

			case Event::Suspend:
				break;

			case Event::DropFile:
			{
				const DropFileEvent* drop = static_cast<const DropFileEvent*>(ev);
				DBG("%s", drop->_filePath.getCPtr());
			}
			break;

			default:
				break;
			}
		}

		InputProcess();

	} while (NULL != ev);

	if (handle.idx == 0
		&& reset != FProcessWindow::_reset)
	{
		reset = FProcessWindow::_reset;
		bgfx::reset(width, height, reset);
		InputSetMouseResolution(uint16_t(width), uint16_t(height));
	}

	debug = FProcessWindow::_debug;

	return FProcessWindow::_exit;
}

}
