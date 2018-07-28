#include "Graphics.h"


AUTO_BEGIN
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
}


Graphics::~Graphics()
{
}

AUTO_END