#include "Auto.h"
#if AUTO_DIRECT_X
#include "Graphics.h"
#include "AutoD3D12.h"
#include "GraphicsDef.h"
#include "NewDef.h"
namespace Auto3D {

void Graphics::RegisterDebug()
{
	
}

bool Graphics::BeginFrame()
{
	
	return true;
}
void Graphics::EndFrame()
{
	
}

void Graphics::Clear(unsigned flags, const Color & color, float depth, unsigned stencil)
{
	
}


void Graphics::SetColorWrite(bool enable)
{
	
}
void Graphics::SetDepthWrite(bool enable)
{
	
}

void Graphics::SetViewport(int posX, int posY, int width, int height)
{

}


void Graphics::CreateGameWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	//MSAA_POINT
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	//SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, MSAA_POINT);



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
}

bool Graphics::IsDeviceLost()
{
	return true;
}

}
#endif