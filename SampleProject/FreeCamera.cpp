#include "FreeCamera.h"
#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"
GameObject* freeCameraObject;
Camera * freeCamera;
bool firstMouse = true;

RectInt rect = INSTANCE(GLWindow).GetWindowRectInt();
float lastX = rect.width;
float lastY = rect.height;

//void mouseCallBack(GLFWwindow* window, double xpos, double ypos)
//{
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//
//	freeCamera->ProcessMouseMovement(INSTANCE(Input).GetMouseMove().x, INSTANCE(Input).GetMouseMove().y);
//}
//void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset)
//{
//	freeCamera->ProcessMouseScroll(yoffset);
//}

void processInput()
{
	
	if (INSTANCE(Input).GetKeyPress(SDLK_w))
		freeCamera->ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (INSTANCE(Input).GetKeyPress(SDLK_s))
		freeCamera->ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime() * 2);
	if (INSTANCE(Input).GetKeyPress(SDLK_a))
		freeCamera->ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime() * 2);
	if (INSTANCE(Input).GetKeyPress(SDLK_d))
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
	freeCamera->SetFar(1000.0f);
	freeCameraObject = new GameObject();
	freeCameraObject->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);

	freeCameraObject->AddComponent(freeCamera);
	SDL_ShowCursor(SDL_DISABLE);
	/*glfwSetCursorPosCallback(INSTANCE(GLWindow).GetGLWindow(), mouseCallBack);
	glfwSetScrollCallback(INSTANCE(GLWindow).GetGLWindow(), scrollCallBack);
	glfwSetInputMode(INSTANCE(GLWindow).GetGLWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);*/
}
void FreeCamera::Update()
{
	processInput();
	if (INSTANCE(Input).IsMouseMove())
	{
		//int MidX = 640 / 2;   //middle of the screen
		//int MidY = 480 / 2;
		//SDL_ShowCursor(SDL_DISABLE);    //we don't show the cursor
		//int tmpx, tmpy;
		//SDL_GetMouseState(&tmpx, &tmpy); //get the current position of the cursor
		//camYaw += mousevel * (MidX - tmpx);   //get the rotation, for example, if the mouse current position is 315, than 5*0.2, this is for Y
		//camPitch += mousevel * (MidY - tmpy); //this is for X
		////SDL_WarpMouse(MidX, MidY);       //move back the cursor to the center of the screen
		
		freeCamera->ProcessMouseMovement(INSTANCE(Input).GetMouseMove().x, INSTANCE(Input).GetMouseMove().y);
	}
	//Print(INSTANCE(Input).GetMouseMove().y);
}

