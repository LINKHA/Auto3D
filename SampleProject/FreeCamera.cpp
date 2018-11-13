#include "FreeCamera.h"
#include "BaseSpace.h"
#include "Time.h"
#include "Light.h"
#include "Input.h"
#include "Scene.h"
#include "Transform.h"
void FreeCamera::processInput()
{
	
	if (GetSubSystem<Input>()->GetKeyPress(KEY_W))
		freeCamera->ProcessKeyboard(CameraMovement::kForward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		freeCamera->ProcessKeyboard(CameraMovement::kBackward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
		freeCamera->ProcessKeyboard(CameraMovement::kLeft, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
		freeCamera->ProcessKeyboard(CameraMovement::kRight, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->IsMouseMove())
	{
		freeCamera->ProcessMouseMovement(GetSubSystem<Input>()->GetMouseMove().x, GetSubSystem<Input>()->GetMouseMove().y);
	}
	freeCamera->ProcessMouseScroll(GetSubSystem<Input>()->GetMouseWheelOffset());
}


FreeCamera::FreeCamera(Ambient* ambient, int levelNumber)
	:ScriptComponent(ambient)
{
	freeCameraObject = new Node(_ambient, levelNumber);
	GetSubSystem<Scene>()->GetLevelScene(levelNumber)->AddNode(freeCameraObject);

	freeCamera = CreateObject<Camera>();
	freeCamera->SetFar(1000.0f);
}
FreeCamera::~FreeCamera()
{
}

void FreeCamera::Start()
{

	freeCameraObject->GetComponent<Transform>()->SetPosition(0.0f, 0.0f, 3.0f);
	freeCameraObject->AddComponent(freeCamera);
	GetSubSystem<Input>()->HideMouseInWindow(true);
	
}
void FreeCamera::Update()
{
	processInput();
	
}
