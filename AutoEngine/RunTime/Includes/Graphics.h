#pragma once
#include "GameManager.h"
#include "Ambient.h"
#include "Math/Color.h"
#include "SDL2/SDL.h"
#include "Auto.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "GraphicsDefines.h"
namespace Auto3D {
class Graphics : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Graphics, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Graphics);
public:
	explicit Graphics(Ambient* ambient);
	/**
	* @brief : Init Graphics create window and context
	*/
	void Init();
	/**
	* @brief : Create a Game window
	*/
	void CreateGameWindow();
	/**
	* @brief : Create Context to draw opengl geometry(Opengl only)
	*/
	void CreateGlContext();
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

	RectInt GetWindowRectInt() { return _windowRect; }
	void SetWindowRectInt(int width, int height) { _windowRect.width = width; _windowRect.height = height; }
	char* GetTitle() { return _titleName; }
	bool GetScreenFullorNot() { return true; }
	SDL_Window* GetGameWindow() { return _window; }


	void Draw(PrimitiveTypes type, unsigned vertexStart, unsigned vertexCount);
	void Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
	void DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,unsigned instanceCount);
	
	bool BeginFrame();
	void EndFrame();	
	/**
	* @brief : Clear color depth and stencil
	*/
	void Clear(unsigned flags , const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
	void SetColorWrite(bool enable);
	void SetDepthWrite(bool enable);
private:
	SDL_Window* _window{};
	SDL_GLContext _context;
	Color _drawColor;
	RectInt _windowRect;
	char* _titleName;
	bool _isFullScreen = false;
	bool _isCenter = true;
	bool _colorWrite{};
	bool _depthWrite{};
	unsigned _stencilWriteMask{};
	unsigned _numPrimitives{};
	unsigned _numBatches{};
};

}
