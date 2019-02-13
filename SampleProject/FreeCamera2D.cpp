#include "FreeCamera2D.h"
#include "Time.h"
#include "tLight.h"
#include "Input.h"
#include "Scene.h"
#include "Transform.h"
void FreeCamera2D::processInput()
{
	if (GetSubSystem<Input>()->GetKeyPress(KEY_W))
		camera->AccPosition(camera->GetUp() * GetSubSystem<Time>()->GetDeltaTime() * speed);
	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
		camera->AccPosition(-camera->GetUp() * GetSubSystem<Time>()->GetDeltaTime() * speed);
	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
		camera->AccPosition(-camera->GetRight() * GetSubSystem<Time>()->GetDeltaTime() * speed);
	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
		camera->AccPosition(camera->GetRight() * GetSubSystem<Time>()->GetDeltaTime() * speed);
}

void FreeCamera2D::Init()
{
	Super::Init();

	cameraNode = CreateNode();
	camera = cameraNode->CreateComponent<Camera>();
	camera->SetType(ProjectionMode::Orthographic);
	camera->SetFar(1000.0f);

	GetSubSystem<Input>()->HideMouseInWindow(true);
}

void FreeCamera2D::Start()
{
	Super::Start();

}
void FreeCamera2D::Update()
{
	Super::Update();

	processInput();

}
