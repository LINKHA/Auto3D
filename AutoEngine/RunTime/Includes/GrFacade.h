#pragma once
#include "Auto.h"
#include "OpenGLGather.h"
#include "LogAssert.h"
#include "Math/Color.h"
#include "../../EngineSetting/Optimize.h"
#define __OPENGL__

USING_MATH
AUTO_BEGIN
/**
* @brief : Init OpenGL Set Version and call GLFW to use (Core-profile) 
* @param : Set Version
*/
static void GrInit(int verMajor,int verMinor)
{
#ifdef __OPENGL__
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, verMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, verMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	
#if MSAA_POINT
	glfwWindowHint(GLFW_SAMPLES, MSAA_POINT);
#endif //MSAA_POINT
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif //__APPLE__

#endif //__OPENGL__

#ifdef __DIRECTX__
#endif

}
/**
* @brief : Create a Window use Point window to set
*/
static void GrCreateWindow(GLFWwindow** window , UInt32 width,UInt32 height, char * titleName,GLFWmonitor * moitor)
{
#ifdef __OPENGL__
	*window = glfwCreateWindow(width, height, titleName, moitor, NULL);

	if (*window == NULL)
	{
		ErrorString("Failed to create GLFW window");
		glfwTerminate();
	}
	//
	glfwMakeContextCurrent(*window);
#endif

#ifdef __DIRECTX__
#endif
	
}
/**
* @brief : call opengl window size
* @param rectX rectY : set Window left down position
*/
static void GrViewPort(UInt32 rectX, UInt32 rectY, UInt32 width, UInt32 height)
{
#ifdef __OPENGL__
	glViewport(rectX, rectY, width, height);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief :get events
*/
static void GrPollEvents()
{
#ifdef __OPENGL__
	glfwPollEvents();
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief : swap buffers function
*/
static void GrSwapBuffers(GLFWwindow* window)
{
	
#ifdef __OPENGL__
	///Exchange color buffer to show in scren
	/// -------------------------------------------------------------------------------
	glfwSwapBuffers(window); glfwPollEvents();
#endif

#ifdef __DIRECTX__
#endif
	
}
/**
* @brief : Accept a buffer bit buffer Bitto specify the buffer 
* to be emptied and Fill in the color set in glClearColor
*/
static void GrClearColor(const Color& color)
{
#ifdef __OPENGL__
	///Fill in the color set in glClearColor
	glClearColor(color.r, color.g, color.b, color.a);
#endif

#ifdef __DIRECTX__
#endif
	
}



/**
* @brief delete all resource
*/
static void GrDeleteResource()
{
#ifdef __OPENGL__
	glfwTerminate();
#endif

#ifdef __DIRECTX__
#endif
}

/**
* @brief : Change window frame size call back function
* @param window : get window point
* @param cbfun : function point
*/
static void GrFrameSizeCallBack(GLFWwindow* window, GLFWframebuffersizefun cbfun)
{
#ifdef __OPENGL__
	glfwSetFramebufferSizeCallback(window, cbfun);
#endif

#ifdef __DIRECTX__
#endif
}
static void GrCursorPosCallback(GLFWwindow* window, GLFWcursorposfun cbfun)
{
#ifdef __OPENGL__
	glfwSetCursorPosCallback(window, cbfun);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief : Close or dont use judge
*/
static bool GrShouldCloseWindow(GLFWwindow* window)
{
#ifdef __OPENGL__
	return glfwWindowShouldClose(window);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief : Close or dont use bool
*/
static void GrSetShouldCloseWindow(GLFWwindow* window, int value)
{
#ifdef __OPENGL__
	glfwSetWindowShouldClose( window,value);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief : Close window
*/
static void GrCloseWindow(GLFWwindow* window)
{
#ifdef __OPENGL__
	glfwSetWindowShouldClose(window, true);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief Get Key return KeyButtonStatus
*/

static int GrGetKey(GLFWwindow* window, int key)
{
#ifdef __OPENGL__
	return glfwGetKey(window, key);
#endif

#ifdef __DIRECTX__
#endif
}

/**
* @brief Get Mouse Key return KeyButtonStatus
*/
static int GrGetMouseButton(GLFWwindow* window, int key)
{
#ifdef __OPENGL__
	return glfwGetMouseButton(window, key);
#endif

#ifdef __DIRECTX__
#endif
}
/**
* @brief Get Time from Applacation load
*/
static double GrGetTime()
{
#ifdef __OPENGL__
	return glfwGetTime();
#endif

#ifdef __DIRECTX__
#endif
}

AUTO_END
