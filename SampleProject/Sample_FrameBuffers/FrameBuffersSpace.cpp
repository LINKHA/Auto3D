#include "FrameBuffersSpace.h"
#include "Application.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "LightPoint.h"
#include "Time.h"
#include "Sprite.h"
FrameBuffersSpace::FrameBuffersSpace(Ambient* ambient)
	:MotionSpace(ambient)
{

}
FrameBuffersSpace::~FrameBuffersSpace()
{}


void FrameBuffersSpace::Start()
{

	GameObject* camObj = new GameObject(_ambient);
	camera = new FreeCamera(_ambient);
	camera->freeCamera->AllowOffScreen(true);
	camera->freeCamera->AllowMSAA(true);
	camera->freeCamera->AllowLateEffect(true);

	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(camera);
	
	Sprite * tex2 = new Sprite(_ambient,"../Resource/texture/window.png");
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);
	GameObject * obj2 = new GameObject(_ambient);
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);


	Sprite * tex5 = new Sprite(_ambient,"../Resource/texture/grass.png");
	tex5->EnableBlend(true);
	GameObject * obj5 = new GameObject(_ambient);
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	GameObject * lightObj = new GameObject(_ambient);
	Light * light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh = new Mesh(_ambient,"../Resource/object/base/Cube.3DS");
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj = new GameObject(_ambient);
	meshObj->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj->AddComponent(mesh);

}

void FrameBuffersSpace::Update()
{
	int i = (int)GetSubSystem<Time>()->GetCurTime() % 6;
	if (oldi != i)
	{
		oldi = i;
		switch (i)
		{
		case 0:
			camera->freeCamera->SetLateEffect(POST_DEFAULT);
			break;
		case 1:
			camera->freeCamera->SetLateEffect(POST_BULR);
			break;
		case 2:
			camera->freeCamera->SetLateEffect(POST_EDGE_DETECTION);
			break;
		case 3:
			camera->freeCamera->SetLateEffect(POST_GRAYSCALE);
			break;
		case 4:
			camera->freeCamera->SetLateEffect(POST_INVERSION);
			break;
		case 5:
			camera->freeCamera->SetLateEffect(POST_SHARPEN);
			break;
		}
	}
		

}
int FrameBuffersSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(FrameBuffersSpace)