#include "Graphics.h"
#if AUTO_DIRECT_X
#include "AutoImage.h"
#include "AutoD3D12.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "NewDef.h"

namespace Auto3D {

void Graphics::Init()
{

}

void Graphics::RegisterDebug()
{
#ifdef _DEBUG
	ComPtr<ID3D12Debug> debugInterface;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugInterface)));
	debugInterface->EnableDebugLayer();
#endif
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
	
}
void Graphics::CreateSamplePoint(int num)
{
	
}

void Graphics::LoadAPILoader()
{
	
}

void Graphics::UpdateSwapChain()
{

}

bool Graphics::IsDeviceLost()
{
	return true;
}

}
#endif