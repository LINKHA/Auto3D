#include "Graphics.h"
#include "stb_image.h"
#include "GLGather.h"



namespace Auto3D {


Graphics::Graphics(Ambient* ambient)
	:Super(ambient)
	, _window(nullptr)
{
	_drawColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	_titleName = "Auto V0.0";
	_windowRect.x = 0;
	_windowRect.y = 0;
	_windowRect.width = 1280;
	_windowRect.height = 720;

	CreateGameWindow();
#if _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_
	CreateGlContext();
#endif
	InitGameWindowPos();

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		AssertString(-1, "Failed to initialize GLAD from Engine");
	}
	RegisterDebug();
	CreateIcon();

	RegisterGraphicsLib(_ambient);
}


Graphics::~Graphics()
{
}

void Graphics::CreateGameWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL); 

#if _OPENGL_4_6_
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#endif // _OPENGL_4_6_

#if  _OPENGL_4_PLUS_ 
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif //  _OPENGL_4_PLUS_

#if _OPENGL_3_PLUS_ 
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif // _OPENGL_3_PLUS_

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
#if _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_
void Graphics::CreateGlContext()
{
	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
		ErrorString("Failed to create OpenGL context");
}
#endif
void Graphics::InitGameWindowPos()
{
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
	///lock cursor in window
	SDL_SetWindowGrab(_window, SDL_TRUE);
}
void Graphics::CreateIcon()
{
	SDL_Surface* surface;
	surface = SetIcon();
	SDL_SetWindowIcon(_window, surface);
	SDL_FreeSurface(surface);
}
SDL_Surface* Graphics::SetIcon()
{
	int req_format = STBI_rgb_alpha;
	int width, height, orig_format;
	
	unsigned char* data = stbi_load("../Resource/texture/logo.png", &width, &height, &orig_format, 0);
	if (!data) {
		SDL_Log("Loading image failed: %s", stbi_failure_reason());
		exit(1);
	}

	// Set up the pixel format color masks for RGB(A) byte arrays.
	// Only STBI_rgb (3) and STBI_rgb_alpha (4) are supported here!
	Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	int shift = (req_format == STBI_rgb) ? 8 : 0;
	rmask = 0xff000000 >> shift;
	gmask = 0x00ff0000 >> shift;
	bmask = 0x0000ff00 >> shift;
	amask = 0x000000ff >> shift;
#else // little endian, like x86
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
	amask = (req_format == STBI_rgb) ? 0 : 0xff000000;
#endif

	int depth, pitch;
	if (req_format == STBI_rgb) {
		depth = 24;
		pitch = 3 * width; // 3 bytes per pixel * pixels per row
	}
	else { // STBI_rgb_alpha (RGBA)
		depth = 32;
		pitch = 4 * width;
	}

	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)data, width, height, depth, pitch,
		rmask, gmask, bmask, amask);

	if (surf == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());
		stbi_image_free(data);
		exit(1);
	}

	return surf;
	// when you don't need the surface anymore, free it..
	//SDL_FreeSurface(surf);
	// .. *and* the data used by the surface!
	//stbi_image_free(data);

}
void Graphics::DestoryWindow()
{
#ifdef  _OPENGL_4_PLUS_
	SDL_GL_DeleteContext(_context);
	_context = nullptr;
#endif //  _OPENGL_4_PLUS_
	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}

bool Graphics::IsInitialized()
{
	return _window != nullptr;
}
bool Graphics::IsDeviceLost()
{
#if defined(IOS) || defined(TVOS)
	if (_window && (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED) != 0)
		return true;
#endif
	return _context == nullptr;
}

void Graphics::RegisterGraphicsLib(Ambient* ambient)
{
	
}

}