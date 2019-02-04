#include "Graphics.h"
#include "AutoSDL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "GPUObject.h"
#include "Image.h"
#include "NewDef.h"
namespace Auto3D {


Graphics::Graphics(SharedPtr<Ambient> ambient)
	:Super(ambient)
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



bool Graphics::IsInitialized()
{
	return _window != nullptr;
}


void Graphics::RegisterGraphicsLib(SharedPtr<Ambient> ambient)
{

}

}