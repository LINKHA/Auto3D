#include "FreeCamera.h"
#include "BaseSpace.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "GameObjectManager.h"
#include "Camera.h"
#include "Light.h"
#include "Input.h"


void FreeCamera::processInput()
{
	
	if (INSTANCE(Input).GetKeyPress(SDLK_w))
		freeCamera->ProcessKeyboard(FORWARD, TimeManager::Instance().GetDeltaTime());
	if (INSTANCE(Input).GetKeyPress(SDLK_s))
		freeCamera->ProcessKeyboard(BACKWARD, TimeManager::Instance().GetDeltaTime());
	if (INSTANCE(Input).GetKeyPress(SDLK_a))
		freeCamera->ProcessKeyboard(LEFT, TimeManager::Instance().GetDeltaTime());
	if (INSTANCE(Input).GetKeyPress(SDLK_d))
		freeCamera->ProcessKeyboard(RIGHT, TimeManager::Instance().GetDeltaTime());
	if (INSTANCE(Input).IsMouseMove())
	{
		freeCamera->ProcessMouseMovement(INSTANCE(Input).GetMouseMove().x, INSTANCE(Input).GetMouseMove().y);
	}
	freeCamera->ProcessMouseScroll(INSTANCE(Input).GetMouseWheelOffset());
}


FreeCamera::FreeCamera(Ambient* ambient)
	:ScriptComponent(ambient)
{
}
FreeCamera::~FreeCamera()
{
}

void FreeCamera::Start()
{
	freeCamera = new Camera(_ambient);
	freeCamera->SetFar(1000.0f);
	freeCameraObject = new GameObject();
	freeCameraObject->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);

	freeCameraObject->AddComponent(freeCamera);

	INSTANCE(Input).ShowCursor(false);
	INSTANCE(Input).LockCursorInCenter();
	
}
void FreeCamera::Update()
{
	processInput();
	
}
