#include "GameWindow.h"
#include "Monitors.h"
#include "Icon.h"
#include "AtConfig.h"
#include "../../EngineSetting/Optimize.h"
AUTO_BEGIN

//public funcation
GameWindow::GameWindow(Ambient* ambient)
	: Super(ambient)
	, _window(nullptr)
{
	_drawColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	_titleName = "Auto V0.0";
	_windowRect.x = 0;
	_windowRect.y = 0;
	_windowRect.width = 1280;
	_windowRect.height = 720;
}

GameWindow::~GameWindow()
{
	SDL_DestroyWindow(_window);
}

void GameWindow::DrawWindow()
{
	glClearColor(_drawColor.r, _drawColor.g, _drawColor.b, _drawColor.a);

}
void GameWindow::RunLoopOver()
{
	SDL_GL_SwapWindow(_window);
}
/**
* @brief delete all resource
*/
void GameWindow::DestoryWindow()
{
	SDL_GL_DeleteContext(_context);
	_context = nullptr;
	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}

void GameWindow::CreateGameWindow()
{


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

	//MSAA
#if MSAA_POINT
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);

	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAA_POINT);
#endif // MSAA_POINT

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

	}

	if (_window == NULL)
		ErrorString("Couldn't set video mode");


	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
		ErrorString("Failed to create OpenGL context");
	SDL_Rect rect;
	SDL_GetDisplayBounds(0, &rect);
	if (_isFullScreen)
	{
		_windowRect.width = rect.w;
		_windowRect.height = rect.h;
	}
	else
	{
		if (_isCenter)
		{
			_windowRect.x = rect.w / 2;
			_windowRect.y = rect.h / 2;
		}
	}
	SDL_SetWindowSize(_window, _windowRect.width, _windowRect.height);
	SDL_SetWindowPosition(_window, _windowRect.x - _windowRect.width / 2, _windowRect.y - _windowRect.height / 2);
}



AUTO_END