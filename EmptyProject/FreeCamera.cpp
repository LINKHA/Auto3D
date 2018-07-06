#include "FreeCamera.h"
#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "BaseLight.h"

Camera *freeCamera;
GameObject* freeCameraObject;

bool firstMouse = true;

RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
float lastX = rect.width;
float lastY = rect.height;

void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
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


	freeCamera->ProcessMouseMovement(xoffset, yoffset);
}
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
{
	freeCamera->ProcessMouseScroll(yoffset);
}

void processInput(GLFWwindow *window)
{
	if (GrGetKey(window, KEY_ESCAPE) == BUTTON_PRESS)
		GrCloseWindow(window);
	if (GrGetKey(window, KEY_W) == BUTTON_PRESS)
		freeCamera->ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_S) == BUTTON_PRESS)
		freeCamera->ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_A) == BUTTON_PRESS)
		freeCamera->ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime() * 2);
	if (GrGetKey(window, KEY_D) == BUTTON_PRESS)
		freeCamera->ProcessKeyboard(RIGHT, TimeManager::Instance().GetDeltaTime() * 2);
}


FreeCamera::FreeCamera()
{
}
FreeCamera::~FreeCamera()
{
}

void FreeCamera::Start()
{
	freeCamera = new Camera(Vector3(0.0f, 0.0f, 3.0f));
	freeCameraObject = new GameObject();
	freeCameraObject->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);

	freeCameraObject->AddComponent(freeCamera);

	glfwSetCursorPosCallback(INSTANCE(GLWindow).GetGLWindow(), mouseCallBack);
	glfwSetScrollCallback(INSTANCE(GLWindow).GetGLWindow(), scrollCallBack);
	glfwSetInputMode(INSTANCE(GLWindow).GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
void FreeCamera::Update()
{
	processInput(INSTANCE(GLWindow).GetGLWindow());
}