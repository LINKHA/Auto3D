#pragma once

#include "../../AutoConfig.h"

#include "../../Container/Ptr.h"

using SDL_GLContext = void *;

namespace Auto3D
{

class Window;

/// OpenGL context associated with a _window, Win32 implementation.
class AUTO_API GraphicsContext : public FRefCounted
{
public:
	/// Construct. Associate with a _window, but do not create the context yet.
	GraphicsContext(Window* window);
	/// Destruct. Destroy the context if created.
	~GraphicsContext();

	/// Create context and initialize extensions. Return true on success. The pixel format can only be chosen once, so a context can not be created more than once to the same _window.
	bool Create();
	/// Present the backbuffer.
	void Present();
	/// Set vsync on/off.
	void SetVSync(bool enable);
	/// Return whether is initialized with a _valid context.
	bool IsInitialized() const { return _contextHandle != nullptr; }
	/// Return gl context
	SDL_GLContext Context() { return _contextHandle; }
private:
	/// Destroy the context.
	void Release();
	/// OpenGL context
	SDL_GLContext _contextHandle;
	/// Associated _window.
	TWeakPtr<Window> _window;

};

}