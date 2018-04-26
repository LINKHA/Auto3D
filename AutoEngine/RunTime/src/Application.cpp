#include "Application.h"
AUTO_BEGIN

template<> Application* Singleton<Application>::m_instance = nullptr;

float lastX = 800 / 2.0;
float lastY = 600 / 2.0;
bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	Application::Instance().m_camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Application::Instance().m_camera.ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
	if (GrGetKey(window, KEY_ESCAPE) == S_PRESS)
		GrCloseWindow(window);

	if (glfwGetKey(window, KEY_W) == S_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(FORWARD, 0.001);
	if (glfwGetKey(window, KEY_S) == S_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(BACKWARD, 0.001);
	if (glfwGetKey(window, KEY_A) == S_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(LEFT, 0.001);
	if (glfwGetKey(window, KEY_D) == S_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(RIGHT, 0.001);
	/*
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(RIGHT, deltaTime);*/

}

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
	m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	Print(Monitors::Instance().getMonitorsCount());
	Print(Monitors::Instance().getMonitorsWidthIndex(1));
	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);

	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD \n");
		return AU_ERROR;
	}
	return AU_NORMAL;
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
