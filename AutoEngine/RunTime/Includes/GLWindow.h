#ifndef GL_WINDOW_H_
#define GL_WINDOW_H_
#include "GrFacade.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"

USING_MATH
AUTO_BEGIN


class GLWindow
{
public:
	GLWindow();
	~GLWindow();
	void DrawWindow();
	void RunLoopOver();
	GLFWwindow* CreateGameWindow();
	void DestoryWindow();
private:
	// member
	GLFWwindow* window;
	Color DrawColor;
	// settings
	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;
	char * Title_Name = "Auto V0.0";


};
AUTO_END
#endif // GL_WINDOW_H_
