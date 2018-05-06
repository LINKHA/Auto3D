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
	void drawWindow();
	void runLoopOver();
	GLFWwindow* createWindow();
	void destoryWindow();
private:
	// member
	GLFWwindow* window;
	Color DrawColor;
	// settings
	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;
	char * Title_Name = "Auto V0.0";
	float m_DrawColorData[4] = {0.5f,1.0f,0.5f,1.0f };


};
AUTO_END
#endif // GL_WINDOW_H_
