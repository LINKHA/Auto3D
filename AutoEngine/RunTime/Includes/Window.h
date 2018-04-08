#ifndef WINDOW_H_
#define WINDOW_H_
#include "GrFacade.h"
#include "AtConfig.h"
#include "Auto.h"
#include "Math/Color.h"

#if WINDOW_DOT_USE
//#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#endif

USING_MATH
AUTO_BEGIN


class Window
{
public:
	Window();
	~Window();
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
	float m_DrawColorData[4] = {0.8f,0.2f,1.0f,1.0f };

};
AUTO_END
#endif // WINDOW_H_
