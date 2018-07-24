#include "GLWindow.h"
#include "Monitors.h"
#include "Icon.h"
#include "AtConfig.h"

AUTO_BEGIN

SINGLETON_INSTANCE(GLWindow);

//void size_callback(GLFWwindow* window, int width, int height)
//{
//	INSTANCE(GLWindow).UpdateWindowRectInt(width, height);
//}

//public funcation
GLWindow::GLWindow()
	: _window(nullptr)
{
	_drawColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	_titleName = "Auto V0.0";
	_windowRect.x = 0;
	_windowRect.y = 0;
	_windowRect.width = 1280;
	_windowRect.height = 720;
}

GLWindow::~GLWindow()
{	
	SDL_DestroyWindow(_window);
}

void GLWindow::DrawWindow()
{
	//GrClearColor(_drawColor);
	glClearColor(_drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);
	
}
void GLWindow::RunLoopOver()
{	
	//GrSwapBuffers(_window);
	SDL_GL_SwapWindow(_window);
}
/**
* @brief delete all resource
*/
void GLWindow::DestoryWindow()
{
	//GrDeleteResource();

}

void GLWindow::CreateGameWindow()
{
	int width, height;
	//width = INSTANCE(Monitors).GetMonitorsWidthIndex(0);
	//height = INSTANCE(Monitors).GetMonitorsHeightWithIndex(0);
	width = 1920;
	height = 1080;
	if (_isFullScreen)
	{
		_windowRect.width = width;
		_windowRect.height = height;
	}
	else
	{
		if (_isCenter)
		{
			_windowRect.x = width / 2;
			_windowRect.y = height / 2;
		}
	}


	//GLFWmonitor* pMonitor = _isFullScreen ? glfwGetPrimaryMonitor() : NULL;
	//GrCreateWindow(&_window, _windowRect.width, _windowRect.height, _titleName, pMonitor);
	//glfwSetWindowPos(_window, _windowRect.x - _windowRect.width/2, _windowRect.y - _windowRect.height / 2);
	//GrFrameSizeCallBack(_window, size_callback);

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL); // Default OpenGL is fine.

							  // Request an OpenGL 4.3 context (should be core)
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// Create the window
	if (_isFullScreen) 
	{
		_window = SDL_CreateWindow(
			_titleName,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL
		);
	}
	else 
	{
		_window = SDL_CreateWindow(
			_titleName,
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			_windowRect.width, _windowRect.height, SDL_WINDOW_OPENGL
		);
		SDL_SetWindowPosition(_window, _windowRect.x - _windowRect.width / 2, _windowRect.y - _windowRect.height / 2);
	}
	if (_window == NULL)
		ErrorString("Couldn't set video mode");

	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
		ErrorString("Failed to create OpenGL context");
	
	
	SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl");
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
}



AUTO_END