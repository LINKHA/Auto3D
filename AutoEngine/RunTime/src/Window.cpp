#include "Window.h"



AUTO_BEGIN

//define funcation
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

//public funcation
Window::Window()
	:window(nullptr)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
	Color tempColor(m_DrawColorData[0], m_DrawColorData[1], m_DrawColorData[2], m_DrawColorData[3]);
	DrawColor = tempColor;
}


Window::~Window()
{
}

void Window::drawWindow()
{
	glClearColor(DrawColor.r, DrawColor.g, DrawColor.b, DrawColor.a);
	glClear(GL_COLOR_BUFFER_BIT);

	// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
	// -------------------------------------------------------------------------------
	glfwSwapBuffers(window);
	glfwPollEvents();
}
GLFWwindow* Window::createWindow()
{
	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, Title_Name, NULL, NULL);

	if (window == NULL)
	{
		//Logger::("Failed to create GLFW window")
		glfwTerminate();
		//Windw Err Close 
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	return window;
}
void Window::destoryWindow()
{
	glfwTerminate();
}


//No member funcation
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
AUTO_END