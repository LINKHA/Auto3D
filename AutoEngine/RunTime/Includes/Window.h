#ifndef WINDOW_H_
#define WINDOW_H_
#include "GLFacade.h"
#include "btConfig.h"
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
	GLFWwindow* createWindow();
	void destoryWindow();
private:
	// member
	GLFWwindow* window;
	Color DrawColor;
	// settings
	unsigned int SCR_WIDTH = 800;
	unsigned int SCR_HEIGHT = 600;
	char * Title_Name = "Bot V1.0";
	float m_DrawColorData[4] = {0.5f,0.5f,1.0f,1.0f };

};
AUTO_END
#endif // WINDOW_H_
