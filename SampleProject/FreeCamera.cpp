//#include "FreeCamera.h"
//#include "Time.h"
//#include "tLight.h"
//#include "Input.h"
//#include "tScene.h"
//#include "Transform.h"
//void FreeCamera::processInput()
//{
//	if (GetSubSystem<Input>()->GetKeyPress(KEY_W))
//		camera->AccPosition(camera->GetFront() * GetSubSystem<Time>()->GetDeltaTime() * speed);
//	if (GetSubSystem<Input>()->GetKeyPress(KEY_S))
//		camera->AccPosition(-camera->GetFront() * GetSubSystem<Time>()->GetDeltaTime() * speed);
//	if (GetSubSystem<Input>()->GetKeyPress(KEY_A))
//		camera->AccPosition(-camera->GetRight() * GetSubSystem<Time>()->GetDeltaTime() * speed);
//	if (GetSubSystem<Input>()->GetKeyPress(KEY_D))
//		camera->AccPosition(camera->GetRight() * GetSubSystem<Time>()->GetDeltaTime() * speed);
//	if (GetSubSystem<Input>()->IsMouseMove())
//	{
//		camera->AccPitchYaw(GetSubSystem<Input>()->GetMouseMove().x * mouseSensitivity, GetSubSystem<Input>()->GetMouseMove().y * mouseSensitivity);
//	}
//	handleMouseWheelOffset();
//}
//void FreeCamera::handleMouseWheelOffset()
//{
//	camera->AccZoom(GetSubSystem<Input>()->GetMouseWheelOffset());
//
//	if (camera->GetZoom() <= limitZoom.x)
//		camera->SetZoom(limitZoom.x);
//	else if (camera->GetZoom() >= limitZoom.y)
//		camera->SetZoom(limitZoom.y);
//}
//void FreeCamera::Init()
//{
//	Super::Init();
//
//	cameraNode = CreateNode();
//	camera = cameraNode->CreateComponent<Camera>();
//
//	camera->SetFar(1000.0f);
//
//	GetSubSystem<Input>()->HideMouseInWindow(true);
//}
//
//void FreeCamera::Start()
//{
//	Super::Start();
//
//}
//void FreeCamera::Update()
//{
//	Super::Update();
//
//	processInput();
//	
//}
