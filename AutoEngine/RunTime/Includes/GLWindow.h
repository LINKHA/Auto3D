#pragma once
#include "GrFacade.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"
#include "Singleton.h"
#include "Math/Rect.h"
USING_MATH
AUTO_BEGIN

class GLWindow : public Singleton<GLWindow>
{
public:
	GLWindow();
	~GLWindow();
	void DrawWindow();
	void RunLoopOver();
	void DestoryWindow();

	void CreateGameWindow();
	GLFWwindow* GetGLWindow() { return _window; }
	RectInt GetWindowRectInt() { return _windowRect; }
	void UpdateWindowRectInt(float width, float height) { _windowRect.width = width; _windowRect.height = height; }
	char* GetTitle() { return _titleName; }
	bool GetScreenFullorNot() { return true; }
private:
	// member
	GLFWwindow* _window;
	Color _drawColor;
	// settings
	RectInt _windowRect;
	char* _titleName;
	bool _isFullScreen = false;
	bool _isCenter = true;
};
AUTO_END

