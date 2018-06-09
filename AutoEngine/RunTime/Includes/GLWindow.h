#ifndef GL_WINDOW_H_
#define GL_WINDOW_H_
#include "GrFacade.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"
#include "Singleton.h"
#include "Math/RectT.h"
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
	GLFWwindow* GetGLWindow() { return window; }
	RectInt GetWindowRectInt() { return WindowRect; }
	void UpdateWindowRectInt(float width, float height) { WindowRect.width = width; WindowRect.height = height; }
	char* GetTitle() { return TitleName; }
	bool GetScreenFullorNot() { return true; }
private:
	// member
	GLFWwindow* window;
	Color DrawColor;
	// settings
	RectInt WindowRect;
	char * TitleName = "Auto V0.0";
	bool isFullScreen = false;
	bool isCenter = true;
};
AUTO_END
#endif // GL_WINDOW_H_
