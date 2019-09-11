#pragma once
#include "../Object/GameManager.h"
#include "../Window/Window.h"
#include "../Graphics/GraphicsContext.h"


using SDL_Event =  union SDL_Event;

namespace Auto3D
{

namespace UIStyleColors
{
	const static Color Text = Color(1.00f, 1.00f, 1.00f, 1.00f);
}

namespace UIFont
{
	const static char* default = "default";
	const static char* standard = "standard";
	const static char* standard_big = "standard_big";
}

class Canvas;

class AUTO_API UI : public BaseModule
{
	REGISTER_OBJECT_CLASS(UI, BaseModule)
public:
	/// Construct.
	UI();
	/// Destruct.
	~UI();

	/// Init UI state
#ifdef AUTO_OPENGL
	bool SetMode(Window* window, GraphicsContext* context);
#else
	bool SetMode(Window* window);
#endif
	/// New frame to draw ui
	bool BeginUI();
	/// Draw data to screen
	void Present();
	/// Render the UI. If renderUICommand is false (default), is assumed to be the default UI render to backbuffer called by Engine, 
	/// and will be performed only once. Additional UI renders to a different rendertarget may be triggered from the renderpath.
	void Render(Canvas* canvas);
	/// Process event to windows
	void ProcessEvent(const SDL_Event* event);
private:
	/// Window this member is assigned in SetMode, make sure that SetMode calls this member later
	WeakPtr<Window> _window;
	/// UI node
	Vector<Canvas*>	_uiNode;
};

/// Register UI related object factories and attributes.
AUTO_API void RegisterUILibrary();

}