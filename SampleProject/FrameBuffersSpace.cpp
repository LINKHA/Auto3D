#include "FrameBuffersSpace.h"
#include "Application.h"
#include "Mesh.h"
#include "FreeCamera.h"
#include "BaseLight.h"
#include "FrameBuffersScreen.h"
#include "TimeManager.h"
FrameBuffersSpace::FrameBuffersSpace()
{
}


FrameBuffersSpace::~FrameBuffersSpace()
{
}
void FrameBuffersSpace::Start()
{
	INSTANCE(FrameBuffersScreen).Enable(true);
	INSTANCE(FrameBuffersScreen).SetEffect(Blur);
	GameObject* camObj = new GameObject();
	FreeCamera* freeCamera = new FreeCamera();
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);

	Texture2D * tex2 = new Texture2D("Resource/texture/window.png");
	tex2->useBlend = true;
	tex2->useDepth = false;
	GameObject * obj2 = new GameObject();
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);

	Texture2D * tex3 = new Texture2D("Resource/texture/window.png");
	tex3->useBlend = true;
	tex3->useDepth = false;
	GameObject * obj3 = new GameObject();
	obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	obj3->AddComponent(tex3);

	Texture2D * tex5 = new Texture2D("Resource/texture/grass.png");
	tex5->useBlend = true;
	GameObject * obj5 = new GameObject();
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	GameObject * lightObj = new GameObject();
	Light * light = new Light(Point);
	//light->ambient.Set(0.1f, 0.1f, 0.1f);
	//light->direction.Set(0.0f, -1.0f, 0.0f);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	GameObject * meshObj = new GameObject();
	meshObj->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj->AddComponent(mesh);

}
int oldi = 0;
void FrameBuffersSpace::Update()
{
	int i =(int)INSTANCE(TimeManager).GetCurTime() % 6;
	if( oldi!= i)
	{
		oldi = i;
		switch (i)
		{
		case 0:
			INSTANCE(FrameBuffersScreen).SetEffect(Default);
			break;
		case 1:
			INSTANCE(FrameBuffersScreen).SetEffect(Blur);
			break;
		case 2:
			INSTANCE(FrameBuffersScreen).SetEffect(Edge_detection);
			break;
		case 3:
			INSTANCE(FrameBuffersScreen).SetEffect(Grayscale);
			break;
		case 4:
			INSTANCE(FrameBuffersScreen).SetEffect(Inversion);
			break;
		case 5:
			INSTANCE(FrameBuffersScreen).SetEffect(Sharpen);
			break;
		}
	}
		

}
int FrameBuffersSpace::Launch()
{
	return INSTANCE(Application).Run();
}
