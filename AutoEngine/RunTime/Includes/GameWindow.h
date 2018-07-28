#pragma once

#include "SDL2/SDL.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"
#include "GameManager.h"
#include "Math/Rect.h"

AUTO_BEGIN
class GameWindow : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(GameWindow, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(GameWindow);
public:
	explicit GameWindow(Ambient* ambient);


	void DrawWindow();
	void RunLoopOver();
	void DestoryWindow();

	void CreateGameWindow();
	SDL_Window* GetGLWindow() { return _window; }
	RectInt GetWindowRectInt() { return _windowRect; }
	void UpdateWindowRectInt(float width, float height) { _windowRect.width = width; _windowRect.height = height; }
	char* GetTitle() { return _titleName; }
	bool GetScreenFullorNot() { return true; }
	SDL_Surface* SetIcon();
	void CreateIcon();
private:
	SDL_Window * _window;
	SDL_GLContext _context;
	Color _drawColor;
	RectInt _windowRect;
	char* _titleName;
	bool _isFullScreen = false;
	bool _isCenter = true;
};
AUTO_END

