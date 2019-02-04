#include "Graphics.h"
#if AUTO_DIRECT_X
#include "AutoImage.h"
#include "AutoD3D12.h"
#include "ResourceSystem.h"
#include "GlobalFactors.h"
#include "Image.h"
#include "NewDef.h"

namespace Auto3D {
	
HWND Graphics::_window = nullptr;

LRESULT CALLBACK Graphics::windowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		//Update();
		//Render();
		break;
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		bool alt = (::GetAsyncKeyState(VK_MENU) & 0x8000) != 0;

		switch (wParam)
		{
		case VK_ESCAPE:
			::PostQuitMessage(0);
			break;
		}
	}
	break;
	// The default window procedure will play a system notification sound 
	// when pressing the Alt+Enter keyboard combination if this message is 
	// not handled.
	case WM_SYSCHAR:
		break;
	case WM_SIZE:
	{
		RECT clientRect = {};
		::GetClientRect(_window, &clientRect);

		int width = clientRect.right - clientRect.left;
		int height = clientRect.bottom - clientRect.top;

		//Resize(width, height);
	}
	break;
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProcW(_window, message, wParam, lParam);
	}

	// Handle any messages the switch statement didn't.
	return DefWindowProc(hWnd, message, wParam, lParam);
}


void Graphics::Init()
{
	::SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
	parseCommandLineArguments();
	RegisterDebug();

	_isTearingSupported = checkTearingSupport();
	RegisterWindowClass(g_hInstance, WSTRING(_titleName).CStr());
}

void Graphics::RegisterWindowClass(HINSTANCE hInst, const wchar_t* windowClassName)
{
	// Register a window class for creating our render window with.
	WNDCLASSEXW windowClass = {};

	windowClass.cbSize = sizeof(WNDCLASSEXW);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = &windowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInst;
	windowClass.hIcon = ::LoadIcon(hInst, NULL); //  MAKEINTRESOURCE(APPLICATION_ICON));
	windowClass.hCursor = ::LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = windowClassName;
	windowClass.hIconSm = ::LoadIcon(hInst, NULL); //  MAKEINTRESOURCE(APPLICATION_ICON));

	static HRESULT hr = ::RegisterClassExW(&windowClass);
	assert(SUCCEEDED(hr));
}
bool Graphics::checkTearingSupport()
{
	BOOL allowTearing = FALSE;

	// Rather than create the DXGI 1.5 factory interface directly, we create the
	// DXGI 1.4 interface and query for the 1.5 interface. This is to enable the 
	// graphics debugging tools which will not support the 1.5 factory interface 
	// until a future update.
	ComPtr<IDXGIFactory4> factory4;
	if (SUCCEEDED(CreateDXGIFactory1(IID_PPV_ARGS(&factory4))))
	{
		ComPtr<IDXGIFactory5> factory5;
		if (SUCCEEDED(factory4.As(&factory5)))
		{
			if (FAILED(factory5->CheckFeatureSupport(
				DXGI_FEATURE_PRESENT_ALLOW_TEARING,
				&allowTearing, sizeof(allowTearing))))
			{
				allowTearing = FALSE;
			}
		}
	}

	return allowTearing == TRUE;
}

void Graphics::parseCommandLineArguments()
{
	int argc;
	wchar_t** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

	for (size_t i = 0; i < argc; ++i)
	{
		if (::wcscmp(argv[i], L"-w") == 0 || ::wcscmp(argv[i], L"--width") == 0)
		{
			_windowRect.width = ::wcstol(argv[++i], nullptr, 10);
		}
		if (::wcscmp(argv[i], L"-h") == 0 || ::wcscmp(argv[i], L"--height") == 0)
		{
			_windowRect.height = ::wcstol(argv[++i], nullptr, 10);
		}
		if (::wcscmp(argv[i], L"-warp") == 0 || ::wcscmp(argv[i], L"--warp") == 0)
		{
			_isUseWarp = true;
		}
	}

	// Free memory allocated by CommandLineToArgvW
	::LocalFree(argv);
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