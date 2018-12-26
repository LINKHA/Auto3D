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
		freeCamera->ProcessKeyboard(CameraMovement::Forward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		freeCamera->ProcessKeyboard(CameraMovement::Backward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
		freeCamera->ProcessKeyboard(CameraMovement::Left, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
		freeCamera->ProcessKeyboard(CameraMovement::Right, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->IsMouseMove())
	{
		freeCamera->ProcessMouseMovement(GetSubSystem<Input>()->GetMouseMove().x, GetSubSystem<Input>()->GetMouseMove().y);
	}
	freeCamera->ProcessMouseScroll(GetSubSystem<Input>()->GetMouseWheelOffset());
}

void FreeCamera::Init()
{
	Super::Init();

	freeCameraObject = new Node(_ambient, _sceneID);
	GetSubSystem<Scene>()->GetLevelScene(_sceneID)->AddNode(freeCameraObject);
}

void FreeCamera::Start()
{
	Super::Start();

	freeCamera = CreateObject<Camera>();
	freeCamera->SetFar(1000.0f);

	freeCameraObject->AddComponent(freeCamera);
	GetSubSystem<Input>()->HideMouseInWindow(true);
}
void FreeCamera::Update()
{
	Super::Update();

	processInput();
	
}
