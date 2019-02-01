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
	* @brief : Load graphics api loader
	*/
	void LoadAPILoader();
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
	void SetWindowRectInt(int width, int height) { _windowRect.width = width; _windowRect.height = height; }
	/**
	* @brief : Get the upper name of the form
	* @return : char*
	*/
	char* GetTitle() { return _titleName; }
	/**
	* @brief : Get screen full or not
	*/
	bool GetScreenFullorNot() { return true; }
	/**
	* @brief : Get game window
	* @return : SDL_Window*
	*/
	SDL_Window* GetGameWindow() { return _window; }
	/**
	* @brief : Create sample point
	*/
	void CreateSamplePoint(int num);
	

	void SetShader(ShaderVariation* vs, ShaderVariation* fs);

	void Draw(PrimitiveTypes type, unsigned vertexStart, unsigned vertexCount);
	void Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
	void DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount, unsigned instanceCount);
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
	void Clear(unsigned flags, const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
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
	/**
	* @brief : Set window view port
	*/
	void SetViewport(int posX, int posY, int width, int height);
	/**
	* @brief : Get graphics api name
	*/
	STRING GetAPIName() const { return _apiName; }
private:
	/// Graphics api name
	STRING _apiName;
#if AUTO_OPENGL
	/// OpenGL context
	SDL_GLContext _glContext;
#elif AUTO_DIRECT_X
	/// Use WARP adapter
	bool _useWarp{};
	/// Direct3D12 device
	ComPtr<ID3D12Device2> _device;
#endif
	/// window
	SDL_Window* _window{};
	/// icon
	SharedPtr<Image> _icon;
	/// background draw color
	Color _drawColor;
	/// window rect
	RectInt _windowRect;
	/// window title name
	char* _titleName;
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
};

}
