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
class RenderSurface;
class VertexBuffer;
class Texture2D;
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
	* @brief : Check supported rendering features
	*/
	void CheckFeatureSupport();
	/**
	* @brief : Get rect with int
	* @return : RectInt
	*/
	RectInt GetWindowRectInt() { return _windowRect; }
	/**
	* @brief :
	*/
	void SetWindowRectInt(int width, int height) { _windowRect.width = width; _windowRect.height = height; }
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

	/// Return whether hardware instancing is supported.
	bool GetInstancingSupport() const { return _instancingSupport; }

	/// Return default texture filtering mode.
	TextureFilterMode GetDefaultTextureFilterMode() const { return _defaultTextureFilterMode; }
	/**
	* @brief : Create sample point
	*/
	void CreateSamplePoint(int num);
	/// Return current rendertarget by index.
	SharedPtr<RenderSurface> GetRenderTarget(unsigned index) const;

	void InitGraphicsState();

	void SetIndexBuffer(SharedPtr<IndexBuffer> buffer);

	void SetShader(ShaderVariation* vs, ShaderVariation* fs);

	void Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount);
	
	void Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
	
	void DrawInstanced(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount, unsigned instanceCount);
	/// Process dirtied state before draw.
	void PrepareDraw();
	/**
	* @brief : Begin to run frame
	*/
	bool BeginFrame();
	/**
	* @brief : Finish end to frame
	*/
	void EndFrame();
	/**
	* @brief : Clear color depth and stencil
	*/
	void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 1.0f), float depth = 1.0f, unsigned stencil = 0);
	/**
	* @brief : Register Graphics library objects.
	*/
	void RegisterGraphicsLib(SharedPtr<Ambient> ambient);
	/**
	* @brief : Set color write
	*/
	void SetColorWrite(bool enable);
	/**
	* @brief : Set depth write
	*/
	void SetDepthWrite(bool enable);
	/// Create a framebuffer using either extension or core functionality. Used only on OpenGL.
	unsigned CreateFramebuffer();
	/// Resolve a multisampled texture on itself.
	bool ResolveToTexture(SharedPtr<Texture2D> texture);

	DepthMode GetDepthTest() const { return _depthTestMode; }
	/// Set hardware culling mode.
	void SetCullMode(CullMode mode);

	void SetDepthTest(DepthMode mode);

	/// Set scissor test.
	void SetScissorTest(bool enable, const RectInt& rect = RectInt(0, 0, 1, 1) , bool borderInclusive = true);
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
	void SetWindowRect(float x, float y) { _windowRect.x = x; _windowRect.y = y; }
	/**
	* @brief : Set window rect with RectInt (only in space awake function)
	*/
	void SetWindowRect(RectInt rect) { _windowRect = rect; }
	/**
	* @brief : Set window rect with Vector2 (only in space awake function)
	*/
	void SetWindowRect(Vector2 vec) { SetWindowRect(vec.x, vec.y); }

	/// Set rendertarget
	void SetRenderTarget(unsigned index, SharedPtr<RenderSurface> renderTarget);
	/// Set rendertarget
	void SetRenderTarget(unsigned index, SharedPtr<Texture2D> texture);
	/// Set depth-stencil surface
	void SetDepthStencil(SharedPtr<RenderSurface> depthStencil);
	/// Set depth-stencil surface
	void SetDepthStencil(SharedPtr<Texture2D> texture);
	/**
	* @brief : Set window view port
	*/
	void SetViewport(const RectInt& rect);
	/// Set texture.
	void SetTexture(unsigned index, SharedPtr<Texture> texture);
	/// Bind texture unit 0 for update. Called by Texture. Used only on OpenGL.
	void SetTextureForUpdate(SharedPtr<Texture> texture);

	void SetSRGB(bool enable);
	/**
	* @brief : Get graphics api name
	*/
	STRING GetAPIName() const { return _apiName; }
	/**
	* @brief : The release of API
	*/
	void ReleaseAPI();
	/// Return hardware format for a compressed image format, or 0 if unsupported.
	unsigned GetFormat(CompressedFormat format) const;
	/// Return shadow map depth texture format, or 0 if not supported.
	unsigned GetShadowMapFormat() const { return _shadowMapFormat; }

	/// Return 24-bit shadow map depth texture format, or 0 if not supported.
	unsigned GetHiresShadowMapFormat() const { return _hiresShadowMapFormat; }

	/// Return whether shadow map depth compare is done in hardware.
	bool GetHardwareShadowSupport() const { return _hardwareShadowSupport; }


	/// Return whether sRGB conversion on texture sampling is supported.
	bool GetSRGBSupport() const { return _sRGBSupport; }
	/// Return current rendertarget width and height.
	Vector2 GetRenderTargetDimensions() const;
	/// Return whether light pre-pass rendering is supported.
	bool GetLightPrepassSupport() const { return _lightPrepassSupport; }
	/// Return whether deferred rendering is supported
	bool GetDeferredSupport() const { return _deferredSupport; }
	/// Mark the FBO needing an update. Used only on OpenGL
	void MarkFBODirty();
	/// Reset all rendertargets, depth-stencil surface and viewport.
	void ResetRenderTargets();
	/// Reset specific rendertarget.
	void ResetRenderTarget(unsigned index);

	void BindFramebuffer(unsigned fbo);
	/// Delete a framebuffer using either extension or core functionality. Used only on OpenGL.
	void DeleteFramebuffer(unsigned fbo);
	/// Bind a framebuffer color attachment using either extension or core functionality. Used only on OpenGL.
	void BindColorAttachment(unsigned index, unsigned target, unsigned object, bool isRenderBuffer);
	/// Bind a framebuffer depth attachment using either extension or core functionality. Used only on OpenGL.
	void BindDepthAttachment(unsigned object, bool isRenderBuffer);
	/// Bind a framebuffer stencil attachment using either extension or core functionality. Used only on OpenGL.
	void BindStencilAttachment(unsigned object, bool isRenderBuffer);

	/// Set vertex attrib divisor. No-op if unsupported. Used only on OpenGL.
	void SetVertexAttribDivisor(unsigned location, unsigned divisor);
#if AUTO_OPENGL
	/**
	* @brief : Restore GPU objects and reinitialize state
	*/
	void Restore();
	/**
	* @brief :  Bind a VBO, avoiding redundant operation
	*/
	void SetVBO(unsigned object);
	/// Bind a UBO, avoiding redundant operation.
	void SetUBO(unsigned object);
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
	/// Implementation.
	SharedPtr<GraphicsImpl> _impl;
#pragma region window
	/// window
	SDL_Window* _window{};
	/// icon
	SharedPtr<Image> _icon{};
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
#pragma endregion

#pragma region Graphics
	/// background draw color
	Color _drawColor = { Color(0.0f, 0.0f, 0.0f, 1.0f) };
	/// color write
	bool _colorWrite{};
	/// depth write
	bool _depthWrite{};
	/// stencil write mask
	unsigned _stencilWriteMask{};
	/// num primitives
	unsigned _numPrimitives{};
	/// num batches
	unsigned _numBatches{};
	/// msaa point num
	unsigned _numSample{};
	/// Depth compare mode.
	DepthMode _depthTestMode{};
	/// Viewport coordinates
	RectInt _viewport;
	/// Vertex buffers in use
	SharedPtr<VertexBuffer> _vertexBuffers[MAX_VERTEX_STREAMS];
	/// Index buffer in use
	SharedPtr<IndexBuffer> _indexBuffer{};
	/// Textures in use
	SharedPtr<Texture> _textures[MAX_TEXTURE_UNITS]{};
	/// Rendertargets in use.
	SharedPtr<RenderSurface> _renderTargets[MAX_RENDERTARGETS]{};

	/// Shadow map depth texture format
	unsigned _shadowMapFormat{};
	/// Hardware shadow map depth compare support flag
	bool _hardwareShadowSupport{};
	/// Shadow map 24-bit depth texture format
	unsigned _hiresShadowMapFormat{};
	/// sRGB conversion on read support flag
	bool _sRGBSupport{};
	/// Light pre-pass rendering support flag
	bool _lightPrepassSupport{};
	/// Deferred rendering support flag
	bool _deferredSupport{};
	/// Instancing support flag
	bool _instancingSupport{};
	/// sRGB conversion on write support flag
	bool _sRGBWriteSupport{};
	/// DXT format support flag
	bool _dxtTextureSupport{};
	/// Anisotropic filtering support flag
	bool _anisotropySupport{};
	/// Default texture filtering mode
	TextureFilterMode _defaultTextureFilterMode{ TextureFilterMode::Trilinear };
	/// Depth-stencil surface in use
	SharedPtr<RenderSurface> _depthStencil{};
	/// Hardware culling mode
	CullMode _cullMode{};
	/// sRGB conversion on write flag for the main window
	bool _sRGB{};
	/// Scissor test rectangle
	RectInt _scissorRect;
	/// Scissor test enable flag
	bool _scissorTest{};
#pragma endregion


};

}
