#include "Application.h"
#include "TimeManager.h"
#include "Sprite.h"
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
	if (GrGetKey(window, KEY_ESCAPE) == BUTTON_PRESS)
		GrCloseWindow(window);
	if (GrGetKey(window, KEY_W) == BUTTON_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_S) == BUTTON_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_A) == BUTTON_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_D) == BUTTON_PRESS)
		Application::Instance().m_camera.ProcessKeyboard(RIGHT, TimeManager::Instance().GetDeltaTime() * 2);
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
int Application::Run()
{
	if (Init() == AU_ERROR)
	{
		return AU_ERROR;
	}
	if (RunLoop() == AU_ERROR)
	{
		return AU_ERROR;
	}
	return Finish();
}

int Application::Init()
{
	stbi_set_flip_vertically_on_load(true);
	m_camera = Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	//Print(Monitors::Instance().getMonitorsCount());
	//Print(Monitors::Instance().getMonitorsWidthIndex(1));
	glfwSetCursorPosCallback(glfwWindow, mouse_callback);
	glfwSetScrollCallback(glfwWindow, scroll_callback);

	glfwSetInputMode(glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		ErrorString("Failed to initialize GLAD from Engine\n");
		return AU_ERROR;
	}
	glEnable(GL_DEPTH_TEST);
	return AU_NORMAL;
}



int Application::RunLoop()
{
	
	Texture2D tex;
	tex.SetColor(Color(0.5f, 0.5f, 0.5f));
	tex.Draw();
	Mesh m("Resource/object/base/Cube.FBX");
	//Mesh m;
	m.Draw();


	while (!GrShouldCloseWindow(glfwWindow))
	{
		TimeManager::Instance().Update();

		//LogString(TimeManager::Instance().GetRealTime().Second);
		//Print(TimeManager::Instance().GetCurTime());
		//Print(TimeManager::Instance().GetDeltaTime());
		//////////////////////////
		processInput(glfwWindow);
		window.DrawWindow();
		///Accept a buffer bit buffer Bitto specify the buffer to be emptied
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		tex.PushToRunloop();
		m.PushToRunloop();


		window.RunLoopOver();
	}


	return AU_NORMAL;
}
int Application::Finish()
{

	window.DestoryWindow();
	glfwWindow = nullptr;
	return AU_NORMAL;
}

Application::Application()
{
	window = GLWindow();
	glfwWindow = window.CreateGameWindow();

}
AUTO_END
