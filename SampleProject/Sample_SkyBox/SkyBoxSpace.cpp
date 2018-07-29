#include "SkyBoxSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "../FreeCamera.h"
#include "SkyBox.h"
#include "Mesh.h"
#include "LightDirectional.h"
#include "SkyBoxReflectMesh.h"
SkyBoxSpace::SkyBoxSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}


SkyBoxSpace::~SkyBoxSpace()
{
}

void SkyBoxSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * skyBoxObj = new GameObject(_ambient);
	SkyBox * skybox = new SkyBox();
	skyBoxObj->AddComponent(skybox);

	GameObject * lightObj = new GameObject(_ambient);
	Light * light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject(_ambient);
	Mesh * mesh = new Mesh(_ambient);
	//mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	mesh->GetMaterial().color = Color(0.0f, 0.0f, 1.0f);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -0.5f);
	meshObj->AddComponent(mesh);

	GameObject * meshObj2 = new GameObject(_ambient);
	SkyBoxReflectMesh * mesh2 = new SkyBoxReflectMesh(_ambient);
	meshObj2->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);

	GameObject * meshObj3 = new GameObject(_ambient);
	SkyBoxReflectMesh * mesh3 = new SkyBoxReflectMesh(_ambient,
		Shader(AtConfig::shader_path + "au_skybox_cube.auvs"
		, AtConfig::shader_path + "au_skybox_cube_refract.aufs"));
	meshObj3->GetComponent(Transform).SetPosition(2.0f, 0.0f, 0.0f);
	meshObj3->AddComponent(mesh3);
}

void SkyBoxSpace::Update()
{
}

int SkyBoxSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(SkyBoxSpace)
