#include "Auto.h"
#ifdef _OPENGL_4_PLUS_
#include "Graphics.h"
#include "OpenGLGather.h"
#include "GLDebug.h"
#include "GraphicsDefines.h"
#include "stb_image.h"
#include "../../EngineSetting/Optimize.h"
AUTO_BEGIN
void Graphics::RegisterDebug()
{
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
}
bool Graphics::BeginFrame()
{
	glEnable(GL_DEPTH);
	glEnable(GL_LESS);
	SetColorWrite(true);
	SetDepthWrite(true);
	Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL);
	return true;
}

void Graphics::CreateGameWindow()
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
}
void Graphics::CreateGlContext()
{
	_context = SDL_GL_CreateContext(_window);
	if (_context == NULL)
		ErrorString("Failed to create OpenGL context");
}
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
	//unsigned char* data = stbi_load((AtConfig::source_path + "Restource/logo.png").c_str(), &width, &height, &orig_format, 0);
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
	SDL_GL_DeleteContext(_context);
	_context = nullptr;
	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}
void Graphics::EndFrame()
{
	SDL_GL_SwapWindow(_window);
}
void Graphics::Clear(unsigned flags, const Color & color, float depth, unsigned stencil)
{
	bool oldColorWrite = _colorWrite;
	bool oldDepthWrite = _depthWrite;

	if (flags & CLEAR_COLOR && !oldColorWrite)
		SetColorWrite(true);
	if (flags & CLEAR_DEPTH && !oldDepthWrite)
		SetDepthWrite(true);
	if (flags & CLEAR_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(MATH_MAX_UNSIGNED);

	unsigned glFlags = 0;
	if (flags & CLEAR_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	if (flags & CLEAR_DEPTH)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & CLEAR_STENCIL)
	{
		glFlags |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(glFlags);
}

void Graphics::SetColorWrite(bool enable)
{
	if (enable != _colorWrite)
	{
		if (enable)
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		else
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		_colorWrite = enable;
	}
}
void Graphics::SetDepthWrite(bool enable)
{
	if (enable != _depthWrite)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
		_depthWrite = enable;
	}
}
AUTO_END
#endif //_OPENGL_4_PLUS_