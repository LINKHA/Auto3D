#include "Application.h"
AUTO_BEGIN

template<> Application* Singleton<Application>::m_instance = nullptr;

Application::~Application()
{
}
/*
* @Application::run
* this is Engine important funcation,
* Determine the number of frames based on the speed of this function
* Run once per frame
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
	Print(Monitors::Instance().getMonitorsCount());
	Print(Monitors::Instance().getMonitorsWidthIndex(1));
	
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
	//Texture2D s;
	//Texture2D d1;
	//Texture2D s1;
	Vector2 vec(0.5f,0.5f);
	//Vector2 vec2(-0.5f, -0.5f);
	//Vector2 vec3(-0.5f, 0.5f);
	//Vector2 vec4(0.5f, -0.5f);
	d.draw(vec);
	//s.draw(vec2);
	//d1.draw(vec3);
	//s1.draw(vec4);

	

	while (!GrShouldCloseWindow(glfwWindow))
	{

		//////////////////////////
		processInput(glfwWindow);

		window.drawWindow();

		d.pushToRunloop();
	//	s.pushToRunloop();
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

Application::Application()
{
	window = GLWindow();
	glfwWindow = window.createWindow();

}
AUTO_END
