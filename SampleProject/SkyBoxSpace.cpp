#include "SkyBoxSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "SkyBox.h"
#include "Mesh.h"
#include "Light.h"
SkyBoxSpace::SkyBoxSpace()
{
}


SkyBoxSpace::~SkyBoxSpace()
{
}

void SkyBoxSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * skyBoxObj = new GameObject();
	SkyBox * skybox = new SkyBox();
	skyBoxObj->AddComponent(skybox);

	GameObject * lightObj = new GameObject();
	Light * light = new Light();
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject();
	Mesh * mesh = new Mesh("Resource/object/base/Cube.FBX");
	mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -1.0f);
	meshObj->AddComponent(mesh);

}

void SkyBoxSpace::Update()
{
}

int SkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run();
}
