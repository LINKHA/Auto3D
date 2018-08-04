#include "ShadowSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "Shadow.h"
#include "ShadowPoint.h"
#include "ShadowTest.h"

ShadowSpace::ShadowSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ShadowSpace::~ShadowSpace()
{}

void ShadowSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	//GameObject * lightObj = new GameObject();
	//Light * light = new LightDirectional();
	//lightObj->AddComponent(light);

	//GameObject * meshObj = new GameObject();
	//Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	//mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	//meshObj->AddComponent(mesh);
	//meshObj->GetComponent(Transform).SetPosition(0.0f, 2.0f, 0.0f);

	GameObject * shadowObj = new GameObject(_ambient);
	Shadow * shadow = new Shadow(_ambient);
	//ShadowPoint * shadow = new ShadowPoint(_ambient);
	shadowObj->AddComponent(shadow);

}

void ShadowSpace::Update()
{
}

int ShadowSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ShadowSpace)