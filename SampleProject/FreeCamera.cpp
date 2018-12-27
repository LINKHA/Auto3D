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
		camera->ProcessKeyboard(CameraMovement::Forward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		camera->ProcessKeyboard(CameraMovement::Backward, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
		camera->ProcessKeyboard(CameraMovement::Left, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
		camera->ProcessKeyboard(CameraMovement::Right, GetSubSystem<Time>()->GetDeltaTime());
	if (GetSubSystem<Input>()->IsMouseMove())
	{
		camera->ProcessMouseMovement(GetSubSystem<Input>()->GetMouseMove().x, GetSubSystem<Input>()->GetMouseMove().y);
	}
	camera->ProcessMouseScroll(GetSubSystem<Input>()->GetMouseWheelOffset());
}

void FreeCamera::Init()
{
	Super::Init();

	cameraNode = CreateNode();
	camera = cameraNode->CreateComponent<Camera>();

	camera->SetFar(1000.0f);

	GetSubSystem<Input>()->HideMouseInWindow(true);
}

void FreeCamera::Start()
{
	Super::Start();

}
void FreeCamera::Update()
{
	Super::Update();

	processInput();
	
}
