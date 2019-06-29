#include "../../AutoConfig.h"

#ifdef AUTO_OPENGL

#include "../../Base/WString.h"
#include "../../Debug/Log.h"
#include "OGLGraphicsContext.h"
#include "../../Window/Window.h"

#include <cstring>
#include <glad.h>

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_scancode.h>
#include <SDL_syswm.h>

#include <Windows.h>
#include <wingdi.h>

#include "../../Debug/DebugNew.h"



namespace Auto3D
{
typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
static PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

GraphicsContext::GraphicsContext(Window* window) :
	_window(window),
	_contextHandle(nullptr)
{
}

GraphicsContext::~GraphicsContext()
{
	Release();
}

bool GraphicsContext::Create()
{
	if (_contextHandle)
		return true;
	if (!_window)
	{
		ErrorString("Window is null, can not create OpenGL context");
		return false;
	}
	_contextHandle = SDL_GL_CreateContext(_window->Handle());
	SDL_GL_MakeCurrent(_window->Handle(), _contextHandle);

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine");
		return false;
	}

	return true;
}

void GraphicsContext::SetVSync(bool enable)
{
	if (_contextHandle && wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable ? 1 : 0);
}

void GraphicsContext::Present()
{
	if (_contextHandle)
		SDL_GL_SwapWindow(_window->Handle());
}

void GraphicsContext::Release()
{
	if (_contextHandle)
	{
		SDL_GL_MakeCurrent(nullptr, nullptr);
		SDL_GL_DeleteContext(_contextHandle);
	}
}

}

#endif
