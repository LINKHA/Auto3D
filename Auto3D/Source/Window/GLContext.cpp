// For conditions of distribution and use, see copyright notice in License.txt

#include "../AutoConfig.h"

#ifdef AUTO_OPENGL

#include "../Base/WString.h"
#include "../Debug/Log.h"
#include "GLContext.h"
#include "Window.h"

#include <cstring>
#include <glad.h>

#include <SDL.h>
#include <SDL_stdinc.h>
#include <SDL_scancode.h>
#include <SDL_syswm.h>

#include <Windows.h>
#include <wingdi.h>

#include "../Debug/DebugNew.h"



namespace Auto3D
{
typedef BOOL(APIENTRY *PFNWGLSWAPINTERVALFARPROC)(int);
static PFNWGLSWAPINTERVALFARPROC wglSwapIntervalEXT = (PFNWGLSWAPINTERVALFARPROC)wglGetProcAddress("wglSwapIntervalEXT");

GLContext::GLContext(Window* window_) :
	window(window_),
	contextHandle(nullptr)
{
}

GLContext::~GLContext()
{
	Release();
}

bool GLContext::Create(int multisample)
{
	if (contextHandle)
		return true;
	if (!window)
	{
		LOGERROR("Window is null, can not create OpenGL context");
		return false;
	}
	contextHandle = SDL_GL_CreateContext(window->Handle());
	SDL_GL_MakeCurrent(window->Handle(), contextHandle);

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		LOGERROR("Failed to initialize GLAD from Engine");
		return false;
	}

	return true;
}

void GLContext::SetVSync(bool enable)
{
	if (contextHandle && wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable ? 1 : 0);
}

void GLContext::Present()
{
	if (contextHandle)
		SDL_GL_SwapWindow(window->Handle());
}

void GLContext::Release()
{
	if (contextHandle)
	{
		SDL_GL_MakeCurrent(nullptr, nullptr);
		SDL_GL_DeleteContext(contextHandle);
	}
}

}

#endif
