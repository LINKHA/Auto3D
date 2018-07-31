#pragma once
#include "GameManager.h"
#include "Ambient.h"
#include "Math/Color.h"
#include "SDL2/SDL.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"
#include "Math/Rect.h"
#include "GraphicsDefines.h"
AUTO_BEGIN
class Graphics : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Graphics, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Graphics);
public:
	explicit Graphics(Ambient* ambient);
	void CreateGameWindow();
	//Opengl only
	void CreateGlContext();

	void InitGameWindowPos();
	void CreateIcon();
	void DestoryWindow();
	SDL_Surface* SetIcon();
	bool IsInitialized();
	bool IsDeviceLost();



	RectInt GetWindowRectInt() { return _windowRect; }
	void UpdateWindowRectInt(float width, float height) { _windowRect.width = width; _windowRect.height = height; }
	char* GetTitle() { return _titleName; }
	bool GetScreenFullorNot() { return true; }
	SDL_Window* GetGameWindow() { return _window; }
	void RegisterDebug();

	void Draw(PrimitiveTypes type, unsigned vertexStart, unsigned vertexCount);
	void Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount);
	void DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,unsigned instanceCount);
	
	bool BeginFrame();
	void EndFrame();	
	void Clear(unsigned flags , const Color& color = Color(0.0f, 0.0f, 0.0f, 0.0f), float depth = 1.0f, unsigned stencil = 0);
	void SetColorWrite(bool enable);
	void SetDepthWrite(bool enable);
private:
	SDL_Window* _window;
	SDL_GLContext _context;
	Color _drawColor;
	RectInt _windowRect;
	char* _titleName;
	bool _isFullScreen = true;
	bool _isCenter = true;


	bool _colorWrite{};
	bool _depthWrite{};
	unsigned _stencilWriteMask{};

	unsigned _numPrimitives{};
	unsigned _numBatches{};
};

AUTO_END
