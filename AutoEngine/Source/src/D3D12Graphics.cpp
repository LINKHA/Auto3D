#include "Graphics.h"
#if AUTO_DIRECT_X
#include "NewDef.h"


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include <d3dx12.h>

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>


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

void Graphics::CreateDevice()
{
	ComPtr<IDXGIAdapter4> dxgiAdapter4 = DxGetAdapter(_useWarp);
	_device = DxCreateDevice(dxgiAdapter4);
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
	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (_isFullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (_isBorderless)
		flags |= SDL_WINDOW_BORDERLESS;
	if (_isResizable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (_isHighDPI)
		flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	// The position size will be reset later
	_window = SDL_CreateWindow(
		_titleName,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		0, 0, flags
	);

	if (_window == NULL)
		ErrorString("Couldn't set video mode");
}

bool Graphics::IsDeviceLost()
{
	return true;
}

}
#endif