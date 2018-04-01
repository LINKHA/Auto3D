#include "Application.h"

AUTO_BEGIN

Application * Application::m_pApplication = nullptr;


Application::~Application()
{
	Assert(this == m_pApplication);
	m_pApplication = nullptr;
}
/*
* @Application::run
* this is Engine important funcation
*/
int Application::run()
{
	if (init() == AU_ERROR)
	{
		return AU_ERROR;
	}
	if (runLoop() == AU_ERROR)
	{
		return AU_ERROR;
	}
	return finish();
}

int Application::init()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD \n");
		return AU_ERROR;
	}
	return AU_NORMAL;
}

void processInput(GLFWwindow *window)
{
	if (GLGetKey(window, KEY_ESCAPE) == S_PRESS)
		GLCloseWindow(window);
}

int Application::runLoop()
{
	while (!GLShouldCloseWindow(glfwWindow))
	{
		processInput(glfwWindow);

		window.drawWindow();
	}
	return AU_NORMAL;
}
int Application::finish()
{
	window.destoryWindow();
	glfwWindow = nullptr;
	return AU_NORMAL;
}

///Static
Application* Application::getInstance()
{
	if (m_pApplication == nullptr)
	{
		m_pApplication = new Application();
	}
	return m_pApplication;
}

///Private
Application::Application()
{
	window = Window();
	glfwWindow = window.createWindow();
	Assert(!m_pApplication);
	m_pApplication = this;
}
AUTO_END
