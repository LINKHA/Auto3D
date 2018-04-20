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
	if (GrGetKey(window, KEY_ESCAPE) == S_PRESS)
		GrCloseWindow(window);
}

int Application::runLoop()
{

	
	Texture2D d;
	Texture2D s;
	//Texture2D d1;
	//Texture2D s1;
	Vector2 vec(0.5f,0.5f);
	Vector2 vec2(-0.5f, -0.5f);
	//Vector2 vec3(-0.5f, 0.5f);
	//Vector2 vec4(0.5f, -0.5f);
	d.draw(vec);
	s.draw(vec2);
	//d1.draw(vec3);
	//s1.draw(vec4);

	

	while (!GrShouldCloseWindow(glfwWindow))
	{

		//////////////////////////
		processInput(glfwWindow);

		window.drawWindow();

		d.pushToRunloop();
		s.pushToRunloop();
		//d1.pushToRunloop();
		//s1.pushToRunloop();

		window.runLoopOver();
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
	window = GLWindow();
	glfwWindow = window.createWindow();
	Assert(!m_pApplication);
	m_pApplication = this;
}
AUTO_END
