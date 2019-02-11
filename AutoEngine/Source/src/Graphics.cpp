#include "Graphics.h"
#include "AutoSDL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "GPUObject.h"
#include "Image.h"
#include "NewDef.h"
namespace Auto3D {



Graphics::~Graphics() = default;

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


bool Graphics::IsInitialized()
{
	return _window != nullptr;
}


void Graphics::RegisterGraphicsLib(SharedPtr<Ambient> ambient)
{

}

}