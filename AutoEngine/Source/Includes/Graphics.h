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
class Framebuffer;
class ConstantBuffer;
///
const static int GRAPHICS_BUFFER_NUM = 3;


/**
* Graphics subsystem. Manages the application window, rendering state and GPU resources
*/
class Graphics : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Graphics, GlobalGameManager)


	using ShaderProgramMap = PAIR_MAP<PAIR<SharedPtr<ShaderVariation>, SharedPtr<ShaderVariation> >, SharedPtr<ShaderProgram> > ;

public:
	explicit Graphics(SharedPtr<Ambient> ambient);
	/// Set graphics mode. Create the window and rendering context if not created yet. Return true on success.
	bool SetMode();

	bool CreateSDLWindow();
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
	void SetWindowRect(int x, int y) { _windowRect._left = x; _windowRect._right = y; }
	/**
	* @brief : Set window rect with RectInt (only in space awake function)
	*/
	void SetWindowRect(RectInt rect) { _windowRect = rect; }
	/**
	* @brief : Set window rect with Vector2 (only in space awake function)
	*/
	void SetWindowRect(Vector2I vec) { SetWindowRect(vec._x, vec._y); }
	
	/// Set multiple color rendertargets and the depth stencil buffer.
	void SetRenderTargets(VECTOR<SharedPtr<Texture> >& renderTargets, SharedPtr<Texture> depthStencil_);
	/**
	* @brief : Get graphics api name
	*/
	STRING GetAPIName() const { return _apiName; }
	/**
	* @brief : The release of API
	*/
	void ReleaseAPI();

	void Close();

	/// Reset rendertarget and depth stencil buffer to the backbuffer.
	void ResetRenderTargets();
	/// Set the viewport to the entire rendertarget or backbuffer.
	void ResetViewport();
	/// Set the color rendertarget and depth stencil buffer.
	void SetRenderTarget(SharedPtr<Texture> renderTarget, SharedPtr<Texture> stencilBuffer);
	/// Set the viewport rectangle. On window resize the viewport will automatically revert to full window.
	void SetViewport(const RectInt& viewport);

	/// Remove all framebuffers except the currently bound one. Called automatically on backbuffer resize, but can also be called manually if you have used rendertarget resolutions or color formats that you will not need any more.
	void CleanupFramebuffers();
#if AUTO_OPENGL

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

	/// Create and initialize the OpenGL context. Return true on success.
	bool createContext(int multisample);

	/// Prepare framebuffer changes.
	void prepareFramebuffer();
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
	bool _isGrab = false;
	/// Stencil test enable flag.
	bool _stencilTest{};
#pragma endregion
	/// GPU objects.
	VECTOR<GPUObject*> _gpuObjects;
	/// Current size of the backbuffer.
	Vector2F _backbufferSize;
	/// Current size of the active rendertarget.
	Vector2F _renderTargetSize;
	/// Helper vector for defining just one color rendertarget.
	VECTOR<SharedPtr<Texture> > _renderTargetVector;
	/// Current mapping of vertex attributes by semantic.
	VECTOR<VECTOR<unsigned> > _attributesBySemantic;
	/// Multisample level.
	int _multisample;
	/// Vertical sync flag.
	bool _vsync;
	/// Enabled vertex attributes bitmask.
	unsigned _enabledVertexAttributes;
	/// Used vertex attributes bitmask.
	unsigned _usedVertexAttributes;
	/// Vertex attribute instancing bitmask for keeping track of divisors.
	unsigned _instancingVertexAttributes;
	/// Bound vertex buffers.
	SharedPtr<VertexBuffer> _vertexBuffers[MAX_VERTEX_STREAMS];
	/// Bound constant buffers by shader stage.
	SharedPtr<ConstantBuffer> _constantBuffers[(int)ShaderStage::Count][MAX_CONSTANT_BUFFERS];
	/// Bound textures by texture unit.
	SharedPtr<Texture> _textures[MAX_TEXTURE_UNITS];
	/// OpenGL active texture targets by texture unit.
	unsigned _textureTargets[MAX_TEXTURE_UNITS];
	
		/// Bound rendertarget textures.
	SharedPtr<Texture> _renderTargets[MAX_RENDERTARGETS];
	/// Current renderstate requested by the application.
	RenderState _currentRenderState;
	/// Renderstate applied to OpenGL.
	RenderState _renderState;
	/// Bound depth-stencil texture.
	SharedPtr<Texture> _depthStencil;
	/// Bound index buffer.
	SharedPtr<IndexBuffer> _indexBuffer;
	/// Bound vertex shader.
	SharedPtr<ShaderVariation> _vertexShader;
	/// Bound pixel shader.
	SharedPtr<ShaderVariation> _pixelShader;
	/// Bound shader program.
	SharedPtr<ShaderProgram> _shaderProgram;
	/// Bound framebuffer object.
	SharedPtr<Framebuffer> _framebuffer;
	/// Current viewport rectangle.
	RectInt _viewport;
	/// Vertex attributes dirty (shader program changed) flag.
	bool _vertexAttributesDirty;
	/// Vertex buffers dirty flag.
	bool _vertexBuffersDirty;
	/// Blend state dirty flag.
	bool _blendStateDirty;
	/// Depth state dirty flag.
	bool _depthStateDirty;
	/// Rasterizer state dirty flag.
	bool _rasterizerStateDirty;
	/// Framebuffer assignment dirty flag.
	bool _framebufferDirty;
	/// Last used OpenGL texture unit.
	size_t _activeTexture;
	/// Last bound vertex buffer object.
	unsigned _boundVBO;
	/// Last bound uniform buffer object.
	unsigned _boundUBO;
	
	size_t _vsConstantBuffers{};

	size_t _psConstantBuffers{};

	/// Shader programs.
	ShaderProgramMap _shaderPrograms;
	/// Framebuffer objects keyed by resolution and color format.
	HASH_MAP<unsigned long long, SharedPtr<Framebuffer> > _framebuffers;
};

}
