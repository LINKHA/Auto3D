#pragma once
#include "GameManager.h"
#include "Ambient.h"
#include "AutoSDL.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "GraphicsDef.h"
#include "Image.h"




namespace Auto3D {

class GPUObject;
class ShaderVariation;

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
#if _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_
	/**
	* @brief : Create Context to draw opengl geometry(Opengl only)
	*/
	void CreateGlContext();
#endif
	/**
	* @brief : Init Game window position(Create window not set position)
	*/
	void InitGameWindowPos();
	/**
	* @brief : Create engine icon in window title
	*/
	void CreateIcon();
	/**
	* @brief : Delete gamewindow and if Opengl delete context
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

	void SetShader(ShaderVariation* vs, ShaderVariation* fs);

	void Draw(PrimitiveTypes type, unsigned vertexStart, unsigned vertexCount);
	void Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
	void DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,unsigned instanceCount);
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
	void Clear(unsigned flags , const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
	/**
	* @brief : Register Graphics library objects.
	*/
	void RegisterGraphicsLib(SharedPtr<Ambient> ambient);

	void SetColorWrite(bool enable);
	
	void SetDepthWrite(bool enable);
	/**
	* @brief : Set title (only in space awake funcation)	
	*/
	void SetTitle(char* title) { _titleName = title; }
	/**
	* @brief : Set icon (only in space awake funcation)	
	*/
	void SetIconImage(Image* icon) { _icon.reset(icon); }
	/**
	* @brief : Set window rect with float (only in space awake funcation)	
	*/
	void SetWindowRect(float x, float y) { _windowRect.x = x; _windowRect.y = y; }
	/**
	* @brief : Set window rect with RectInt (only in space awake funcation)	
	*/
	void SetWindowRect(RectInt rect) { _windowRect = rect; }
	/**
	* @brief : Set window rect with Vector2 (only in space awake funcation)	
	*/
	void SetWindowRect(Vector2 vec) { SetWindowRect(vec.x, vec.y); }
private:
#if _OPENGL_4_6_ || _OPENGL_4_PLUS_ || _OPENGL_3_PLUS_
	/// opengl context
	SDL_GLContext _context;
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
	/// window in screen flag
	bool _isCenter = true;
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
};

}
