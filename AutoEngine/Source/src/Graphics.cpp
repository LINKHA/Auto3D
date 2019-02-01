#include "Graphics.h"
#include "AutoImage.h"
#include "AutoOGL.h"
#include "AutoSDL.h"
#include "ResourceSystem.h"
#include "GPUObject.h"
#include "NewDef.h"
namespace Auto3D {


Graphics::Graphics(SharedPtr<Ambient> ambient)
	:Super(ambient)
	, _window(nullptr)
#if _OPENGL_4_6_
	, _apiName("OpenGL 4.6")
#elif _OPENGL_4_PLUS_
	, _apiName("OpenGL 4.3")
#elif _OPENGL_3_PLUS_
	, _apiName("OpenGL 3.3")
#elif _DIRECT_3D_12
	, _apiName("Direct3D 12")
#else
	, _apiName("UnKnow")
#endif
{
	_drawColor.Set(0.0f, 0.0f, 0.0f, 0.0f);
	_titleName = "Auto V0.0";
	_windowRect.x = 0;
	_windowRect.y = 0;
	_windowRect.width = 1280;
	_windowRect.height = 720;
	RegisterGraphicsLib(_ambient);
}


Graphics::~Graphics() = default;

void Graphics::Init()
{
	_icon = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");

	stbi_set_flip_vertically_on_load(true);

	CreateGameWindow();

	CreateDevice();

	InitGameWindowPos();

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		AssertString(-1, "Failed to initialize GLAD from Engine");
	}
	RegisterDebug();
	CreateIcon();
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
	//int width, height, orig_format;

	//unsigned char* data = stbi_load(TITLE_ICON_PATH, &width, &height, &orig_format, 0);
	//_icon->GetData();
	if (!_icon->GetData()) {
		SDL_Log("Loading image failed: %s", stbi_failure_reason());
		Assert(0);
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
		pitch = 3 * _icon->GetWidth(); // 3 bytes per pixel * pixels per row
	}
	else { // STBI_rgb_alpha (RGBA)
		depth = 32;
		pitch = 4 * _icon->GetWidth();
	}

	SDL_Surface* surf = SDL_CreateRGBSurfaceFrom((void*)_icon->GetData().get(), _icon->GetWidth(), _icon->GetHeight(), depth, pitch,
		rmask, gmask, bmask, amask);

	if (surf == NULL) {
		SDL_Log("Creating surface failed: %s", SDL_GetError());
		//stbi_image_free(data);
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
#if AUTO_OPENGL
	SDL_GL_DeleteContext(_glContext);
	_glContext = nullptr;
#endif //  _OPENGL_4_PLUS_
	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}

bool Graphics::IsInitialized()
{
	return _window != nullptr;
}


void Graphics::RegisterGraphicsLib(SharedPtr<Ambient> ambient)
{

}

}