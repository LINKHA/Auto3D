#include "FrameBuffersSpace.h"
#include "Application.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "LightPoint.h"
#include "FrameBuffersScreen.h"
#include "Time.h"
#include "Sprite.h"
FrameBuffersSpace::FrameBuffersSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
FrameBuffersSpace::~FrameBuffersSpace()
{}


void FrameBuffersSpace::Start()
{
	glEnable(GL_FRAMEBUFFER_SRGB);
	GetSubSystem<FrameBuffersScreen>()->Enable(true);

	GameObject* camObj = new GameObject(_ambient);
	FreeCamera* freeCamera = new FreeCamera(_ambient);
	camObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	camObj->AddComponent(freeCamera);
	
	Sprite * tex2 = new Sprite(_ambient,"../Resource/texture/window.png");
	tex2->EnableBlend(true);
	tex2->EnableDepth(false);
	GameObject * obj2 = new GameObject(_ambient);
	obj2->GetComponent(Transform).SetPosition(0.0f, 0.0f, 0.0f);
	obj2->AddComponent(tex2);

	Sprite * tex3 = new Sprite(_ambient,"../Resource/texture/window.png");
	tex3->EnableBlend(true);
	tex3->EnableDepth(false);
	GameObject * obj3 = new GameObject(_ambient);
	obj3->GetComponent(Transform).SetPosition(0.2f, 0.0f, -1.0f);
	obj3->AddComponent(tex3);

	Sprite * tex5 = new Sprite(_ambient,"../Resource/texture/grass.png");
	tex5->EnableBlend(true);
	GameObject * obj5 = new GameObject(_ambient);
	obj5->GetComponent(Transform).SetPosition(-0.2f, 0.0f, -3.0f);
	obj5->AddComponent(tex5);

	GameObject * lightObj = new GameObject(_ambient);
	Light * light = new LightPoint(_ambient);
	lightObj->AddComponent(light);
	//////////////////////////////////////////////////////////////////////////
	Mesh * mesh = new Mesh(_ambient,"../Resource/object/base/Cube.FBX");
	mesh->GetMaterial().color.Set(0.5f, 0.8f, 0.3f);
	//mesh->GetMaterial().SetImage("Resource/texture/window.png");
	GameObject * meshObj = new GameObject(_ambient);
	meshObj->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj->AddComponent(mesh);

}
int oldi = 0;
void FrameBuffersSpace::Update()
{
	int i =(int)GetSubSystem<Time>()->GetCurTime() % 6;
	Print(i);
	if( oldi!= i)
	{
		oldi = i;
		switch (i)
		{
		case 0:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kDefault);
			break;
		case 1:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kBlur);
			break;
		case 2:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kEdge_detection);
			break;
		case 3:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kGrayscale);
			break;
		case 4:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kInversion);
			break;
		case 5:
			GetSubSystem<FrameBuffersScreen>()->SetEffect(kSharpen);
			break;
		}
	}
		

}
int FrameBuffersSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(FrameBuffersSpace)