#include "Graphics.h"
#if AUTO_DIRECT_X
#include "AutoImage.h"
#include "AutoD3D12.h"
#include "ResourceSystem.h"
#include "Image.h"

#include "NewDef.h"

#if defined(min)
#undef min
#endif

#if defined(max)
#undef max
#endif

namespace Auto3D {

static HWND GetWindowHandle(SDL_Window* window)
{
	SDL_SysWMinfo sysInfo;

	SDL_VERSION(&sysInfo.version);
	SDL_GetWindowWMInfo(window, &sysInfo);
	return sysInfo.info.win.window;
}


bool CheckTearingSupport()
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

uint64_t Signal(ComPtr<ID3D12CommandQueue> commandQueue, ComPtr<ID3D12Fence> fence,
	uint64_t& fenceValue)
{
	uint64_t fenceValueForSignal = ++fenceValue;
	ThrowIfFailed(commandQueue->Signal(fence.Get(), fenceValueForSignal));

	return fenceValueForSignal;
}

void WaitForFenceValue(ComPtr<ID3D12Fence> fence, uint64_t fenceValue, HANDLE fenceEvent,
	std::chrono::milliseconds duration = std::chrono::milliseconds::max())
{
	if (fence->GetCompletedValue() < fenceValue)
	{
		ThrowIfFailed(fence->SetEventOnCompletion(fenceValue, fenceEvent));
		::WaitForSingleObject(fenceEvent, static_cast<DWORD>(duration.count()));
	}
}

void Graphics::Init()
{
	_icon = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");

	stbi_set_flip_vertically_on_load(true);
	// Create game window
	CreateGameWindow();
	// Create device(OpenGL context and DirectX device)
	CreateDevice();
	// Load graphics API with SDL address
	LoadAPILoader();
	// Register graphics API debug
	RegisterDebug();
	// Create Icon
	CreateIcon();
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

	_commandQueue = DxCreateCommandQueue(_device, D3D12_COMMAND_LIST_TYPE_DIRECT);

	_swapChain = CreateSwapChain(_commandQueue, 500, 500, _numFrames);

	_currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();

	_RTVDescriptorHeap = DxCreateDescriptorHeap(_device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, _numFrames);
	_RTVDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	UpdateRenderTargetViews(_device, _swapChain, _RTVDescriptorHeap);

	for (int i = 0; i < _numFrames; ++i)
	{
		_commandAllocators[i] = DxCreateCommandAllocator(_device, D3D12_COMMAND_LIST_TYPE_DIRECT);
	}
	_commandList = DxCreateCommandList(_device,
		_commandAllocators[_currentBackBufferIndex], D3D12_COMMAND_LIST_TYPE_DIRECT);

	_fence = DxCreateFence(_device);
	_fenceEvent = DxCreateEventHandle();
	Print("For the sake of demonstration, loop again temporarily");
	while (1)
	{
		auto commandAllocator = _commandAllocators[_currentBackBufferIndex];
		auto backBuffer = _backBuffers[_currentBackBufferIndex];

		commandAllocator->Reset();
		_commandList->Reset(commandAllocator.Get(), nullptr);

		// Clear the render target.
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				backBuffer.Get(),
				D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);

			_commandList->ResourceBarrier(1, &barrier);

			FLOAT clearColor[] = { 0.4f, 0.6f, 0.9f, 1.0f };
			CD3DX12_CPU_DESCRIPTOR_HANDLE rtv(_RTVDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
				_currentBackBufferIndex, _RTVDescriptorSize);

			_commandList->ClearRenderTargetView(rtv, clearColor, 0, nullptr);
		}

		// Present
		{
			CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
				backBuffer.Get(),
				D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
			_commandList->ResourceBarrier(1, &barrier);

			ThrowIfFailed(_commandList->Close());

			ID3D12CommandList* const commandLists[] = {
				_commandList.Get()
			};
			_commandQueue->ExecuteCommandLists(_countof(commandLists), commandLists);

			_frameFenceValues[_currentBackBufferIndex] = Signal(_commandQueue, _fence, _fenceValue);

			UINT syncInterval = _VSync ? 1 : 0;
			UINT presentFlags = _tearingSupported && !_VSync ? DXGI_PRESENT_ALLOW_TEARING : 0;
			ThrowIfFailed(_swapChain->Present(syncInterval, presentFlags));

			_currentBackBufferIndex = _swapChain->GetCurrentBackBufferIndex();

			WaitForFenceValue(_fence, _frameFenceValues[_currentBackBufferIndex], _fenceEvent);
		}
	}
}

ComPtr<IDXGISwapChain4> Graphics::CreateSwapChain(ComPtr<ID3D12CommandQueue> commandQueue,
	uint32_t width, uint32_t height, uint32_t bufferCount)
{
	ComPtr<IDXGISwapChain4> dxgiSwapChain4;
	ComPtr<IDXGIFactory4> dxgiFactory4;
	UINT createFactoryFlags = 0;
#if defined(_DEBUG)
	createFactoryFlags = DXGI_CREATE_FACTORY_DEBUG;
#endif

	ThrowIfFailed(CreateDXGIFactory2(createFactoryFlags, IID_PPV_ARGS(&dxgiFactory4)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.Stereo = FALSE;
	swapChainDesc.SampleDesc = { 1, 0 };
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = bufferCount;
	swapChainDesc.Scaling = DXGI_SCALING_STRETCH;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
	// It is recommended to always allow tearing if tearing support is available.
	swapChainDesc.Flags = CheckTearingSupport() ? DXGI_SWAP_CHAIN_FLAG_ALLOW_TEARING : 0;

	ComPtr<IDXGISwapChain1> swapChain1;
	ThrowIfFailed(dxgiFactory4->CreateSwapChainForHwnd(
		commandQueue.Get(),
		GetWindowHandle(_window),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain1));

	// Disable the Alt+Enter fullscreen toggle feature. Switching to fullscreen
	// will be handled manually.
	ThrowIfFailed(dxgiFactory4->MakeWindowAssociation(GetWindowHandle(_window), DXGI_MWA_NO_ALT_ENTER));

	ThrowIfFailed(swapChain1.As(&dxgiSwapChain4));

	return dxgiSwapChain4;
}

void Graphics::UpdateRenderTargetViews(ComPtr<ID3D12Device2> device,
	ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap)
{
	auto rtvDescriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(descriptorHeap->GetCPUDescriptorHandleForHeapStart());

	for (int i = 0; i < _numFrames; ++i)
	{
		ComPtr<ID3D12Resource> backBuffer;
		ThrowIfFailed(swapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)));

		device->CreateRenderTargetView(backBuffer.Get(), nullptr, rtvHandle);

		_backBuffers[i] = backBuffer;

		rtvHandle.Offset(rtvDescriptorSize);
	}
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

	// MSAA sample point
	CreateSamplePoint(_numSample);

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

	// Init window position
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

	// Lock cursor in window
	if (_isGrab)
		SDL_SetWindowGrab(_window, SDL_TRUE);
	else
		SDL_SetWindowGrab(_window, SDL_FALSE);
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
	return false;
}

}
#endif