#pragma once
#include "GameManager.h"
#include "GraphicsDef.h"
#include "AutoSDL.h"
#include "Image.h"
#if AUTO_DIRECT_X
#	include "AutoD3D12.h"
#endif

namespace Auto3D {
class GPUObject;
class ShaderVariation;
class GraphicsImpl;
class IndexBuffer;
class Texture;
class VertexBuffer;
class ShaderProgram;
///
const static int GRAPHICS_BUFFER_NUM = 3;

/**
* Graphics subsystem. Manages the application window, rendering state and GPU resources
*/
class Graphics : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Graphics, GlobalGameManager)
public:
	explicit Graphics(SharedPtr<Ambient> ambient);
	/**
	* @brief : Graphics init
	*/
	void Init();
	/**
	* @brief : Create a Game window
	*/
	void CreateGameWindow();
	/**
	* @brief : Create the  device and swap chain. (DirectX and OpenGL)
	*/
	void CreateDevice();
	/**
	* @brief : Create engine icon in window title
	*/
	void CreateIcon();
	/**
	* @brief : Delete game window and if OpenGL delete context
	*/
	void DestoryWindow();
	/**
	* @brief : Set icon use to create icon
	* @return : icon surface
	*/
	SDL_Surface* SetIcon();
	/**
	* @brief : Register graphics debug function
	*/
	void RegisterDebug();
	/**
	* @brief : Update swap chain(Direct3D 12 only)
	*/
	void UpdateSwapChain();
	/**
	* @brief : Determine whether the graphics is initialized
	*/
	bool IsInitialized();
	/**
	* @brief : Return context lost status
	*/
	bool IsDeviceLost();
	/**
	* @brief : Get rect with int
	* @return : RectInt
	*/
	RectInt GetWindowRectInt() { return _windowRect; }
	/**
	* @brief :
	*/
	void SetWindowRectInt(int width, int height) { _windowRect._right = width + _windowRect._left; _windowRect._top = height + _windowRect._bottom; }
	/**
	* @brief : Get the upper name of the form
	* @return : char*
	*/
	STRING GetTitle() { return _titleName; }
	/**
	* @brief : Get screen full or not
	*/
	bool GetScreenFullorNot() { return true; }
	/**
	* @brief : Get game window
	* @return : SDL_Window*
	*/
	SDL_Window* GetGameWindow() { return _window; }

	void CreateSamplePoint(int num);

	/**
	* @brief : Begin to run frame
	*/
	bool BeginFrame();
	/**
	* @brief : Finish end to frame
	*/
	void EndFrame();
	/**
	* @brief : Reset cached rendering state
	*/
	void ResetCachedState();
	/**
	* @brief : Set title (only in space awake function)
	*/
	void SetTitle(char* title) { _titleName = title; }
	/**
	* @brief : Set icon (only in space awake function)
	*/
	void SetIconImage(Image* icon) { _icon.reset(icon); }
	/**
	* @brief : Set window rect with float (only in space awake function)
	*/
	void SetWindowRect(float x, float y) { _windowRect._left = x; _windowRect._right = y; }
	/**
	* @brief : Set window rect with RectInt (only in space awake function)
	*/
	void SetWindowRect(RectInt rect) { _windowRect = rect; }
	/**
	* @brief : Set window rect with Vector2 (only in space awake function)
	*/
	void SetWindowRect(Vector2 vec) { SetWindowRect(vec.x, vec.y); }

	/**
	* @brief : Get graphics api name
	*/
	STRING GetAPIName() const { return _apiName; }
	/**
	* @brief : The release of API
	*/
	void ReleaseAPI();

#if AUTO_OPENGL
	/**
	* @brief : Restore GPU objects and reinitialize state
	*/
	void Restore();
#endif
	/// Return the API-specific alpha texture format
	static unsigned GetAlphaFormat();
	/// Return the API-specific luminance texture format
	static unsigned GetLuminanceFormat();
	/// Return the API-specific luminance alpha texture format
	static unsigned GetLuminanceAlphaFormat();
	/// Return the API-specific RGB texture format
	static unsigned GetRGBFormat();
	/// Return the API-specific RGBA texture format
	static unsigned GetRGBAFormat();
	/// Return the API-specific RGBA 16-bit texture format
	static unsigned GetRGBA16Format();
	/// Return the API-specific RGBA 16-bit float texture format
	static unsigned GetRGBAFloat16Format();
	/// Return the API-specific RGBA 32-bit float texture format
	static unsigned GetRGBAFloat32Format();
	/// Return the API-specific RG 16-bit texture format
	static unsigned GetRG16Format();
	/// Return the API-specific RG 16-bit float texture format
	static unsigned GetRGFloat16Format();
	/// Return the API-specific RG 32-bit float texture format
	static unsigned GetRGFloat32Format();
	/// Return the API-specific single channel 16-bit float texture format
	static unsigned GetFloat16Format();
	/// Return the API-specific single channel 32-bit float texture format
	static unsigned GetFloat32Format();
	/// Return the API-specific linear depth texture format
	static unsigned GetLinearDepthFormat();
	/// Return the API-specific hardware depth-stencil texture format
	static unsigned GetDepthStencilFormat();
	/// Return the API-specific readable hardware depth format, or 0 if not supported
	static unsigned GetReadableDepthFormat();
	/// Return the API-specific texture format from a textual description, for example "rgb".
	static unsigned GetFormat(const STRING& formatName);
private:
#if AUTO_DIRECT_X
	ComPtr<IDXGISwapChain4> createSwapChain(ComPtr<ID3D12CommandQueue> commandQueue,
		uint32_t width, uint32_t height, uint32_t bufferCount);

	void updateRenderTargetViews(ComPtr<ID3D12Device2> device,
		ComPtr<IDXGISwapChain4> swapChain, ComPtr<ID3D12DescriptorHeap> descriptorHeap);
#endif
private:

#if AUTO_OPENGL
	/// OpenGL only VAO
	unsigned _vertexArrayObject;
#elif AUTO_DIRECT_X
	
	/// Direct3D12 device
	ComPtr<ID3D12Device2> _device;

	ComPtr<ID3D12CommandQueue> _commandQueue;

	ComPtr<IDXGISwapChain4> _swapChain;

	UINT _currentBackBufferIndex;

	ComPtr<ID3D12Resource> _currentBackBuffer;

	ComPtr<ID3D12Resource> _backBuffers[GRAPHICS_BUFFER_NUM];

	ComPtr<ID3D12CommandAllocator> _currentCommandAllocator;

	ComPtr<ID3D12CommandAllocator> _commandAllocators[GRAPHICS_BUFFER_NUM];

	uint64_t _frameFenceValues[GRAPHICS_BUFFER_NUM] = {};

	ComPtr<ID3D12DescriptorHeap> _RTVDescriptorHeap;

	UINT _RTVDescriptorSize;

	ComPtr<ID3D12GraphicsCommandList> _commandList;

	ComPtr<ID3D12Fence> _fence;

	uint64_t _fenceValue = 0;

	HANDLE _fenceEvent;

	bool _VSync = true;

	bool _tearingSupported = false;
	/// Use WARP adapter
	bool _useWarp{};
#endif
	/// Graphics api name
	STRING _apiName{};
	/// Graphics driver
	STRING _driverName{};
#pragma region window
	/// window
	SDL_Window* _window{};
	/// icon
	SharedPtr<Image> _icon{};

	SDL_GLContext _glContext;
	/// window rect
	RectInt _windowRect = { RectInt(0, 0, 1280, 720) };
	/// window title name
	STRING _titleName = "Auto V0.0";
	/// full screen flag
	bool _isFullScreen = false;
	/// border less flag
	bool _isBorderless = false;
	/// resize enable flag
	bool _isResizable = false;
	/// high DPI flag
	bool _isHighDPI = false;
	/// window in screen flag
	bool _isCenter = true;
	/// cursor lock in screen flag
	bool _isGrab = true;
	/// Stencil test enable flag.
	bool _stencilTest{};
#pragma endregion



};

}
