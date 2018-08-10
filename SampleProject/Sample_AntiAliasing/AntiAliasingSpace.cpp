#include "AntiAliasingSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "Time.h"


AntiAliasingSpace::AntiAliasingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
AntiAliasingSpace::~AntiAliasingSpace()
{}

void AntiAliasingSpace::Start()
{
	GameObject* cameraObj = new GameObject(_ambient);
	camera = new FreeCamera(_ambient);
	camera->freeCamera->AllowMSAA(true,4);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject* lightObj = new GameObject(_ambient);
	Light* light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject* meshObj = new GameObject(_ambient);
	Mesh* mesh = new Mesh(_ambient,"../Resource/object/base/Cube.3DS");
	meshObj->AddComponent(mesh);

}

void AntiAliasingSpace::Update()
{
	int i =(int)GetSubSystem<Time>()->GetCurTime() % 6;
	//Print(i);
	if( oldi!= i)
	{
		oldi = i;
		switch (i)
		{
		case 0:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kDefault);
			camera->freeCamera->SetPostProcess(POST_DEFAULT);
			break;
		case 1:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kBlur);
			camera->freeCamera->SetPostProcess(POST_BULR);
			break;
		case 2:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kEdge_detection);
			camera->freeCamera->SetPostProcess(POST_EDGE_DETECTION);
			break;
		case 3:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kGrayscale);
			camera->freeCamera->SetPostProcess(POST_GRAYSCALE);
			break;
		case 4:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kInversion);
			camera->freeCamera->SetPostProcess(POST_INVERSION);
			break;
		case 5:
			//GetSubSystem<FrameBuffersScreen>()->SetEffect(kSharpen);
			camera->freeCamera->SetPostProcess(POST_SHARPEN);
			break;
		}
	}
}

int AntiAliasingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(AntiAliasingSpace)