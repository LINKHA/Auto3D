#ifndef GL_FACADE_H_
#define GL_FACADE_H_
#include "Auto.h"
#include "OpenGLGather.h"
#include "LogAssert.h"
#include "Math/Color.h"

USING_MATH
AUTO_BEGIN
/**
* @brief : Init OpenGL Set Version and call GLFW to use (Core-profile) 
* @param : Set Version
*/
static void GLInit(int verMajor,int verMinor)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, verMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, verMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}
/**
* @brief : Create a Window use Point window to set
*/
static void GLCreateWindow(GLFWwindow** window , UInt32 width,UInt32 height, char * titleName)
{
	*window = glfwCreateWindow(width, height, titleName, NULL, NULL);

	if (*window == NULL)
	{
		ErrorString("Failed to create GLFW window");
		glfwTerminate();
	}
	//
	glfwMakeContextCurrent(*window);
}
/**
* @brief : call opengl window size
* @param rectX rectY : set Window left down position
*/
static void GLViewPort(UInt32 rectX, UInt32 rectY, UInt32 width, UInt32 height)
{
	glViewport(rectX, rectY, width, height);
}
/**
* @brief : runloop needful function
*/
static void GLRunApplication(GLFWwindow* window)
{
	///Exchange color buffer to show in scren
	/// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	///get event
	/// -------------------------------------------------------------------------------
	glfwPollEvents();
}
/**
* @brief : Accept a buffer bit buffer Bitto specify the buffer 
* to be emptied and Fill in the color set in glClearColor
*/
static void GLClearColor(const Color& color)
{
	///Fill in the color set in glClearColor
	glClearColor(color.r, color.g, color.b, color.a);
	///Accept a buffer bit buffer Bitto specify the buffer to be emptied
	glClear(GL_COLOR_BUFFER_BIT);
}



/**
* @brief delete all resource
*/
static void GLDeleteResource()
{
	glfwTerminate();
}

/**
* @brief : Change window frame size call back function
* @param window : get window point
* @param cbfun : function point
*/
static void GLFrameSizeCallBack(GLFWwindow* window, GLFWframebuffersizefun cbfun)
{
	glfwSetFramebufferSizeCallback(window, cbfun);
}
/**
* @brief : Close or dont use judge
*/
static bool GLShouldCloseWindow(GLFWwindow* window)
{
	return glfwWindowShouldClose(window);
}
/**
* @brief : Close or dont use bool
*/
static void GLSetShouldCloseWindow(GLFWwindow* window, int value)
{
	glfwSetWindowShouldClose( window,value);
}
/**
* @brief : Close window
*/
static void GLCloseWindow(GLFWwindow* window)
{
	glfwSetWindowShouldClose(window, true);
}
/**
* @brief Get Key return KeyButtonStatus
*/
static int GLGetKey(GLFWwindow* window,int key)
{
	return glfwGetKey(window, key);
}



AUTO_END

#endif // !GL_FACADE_H_