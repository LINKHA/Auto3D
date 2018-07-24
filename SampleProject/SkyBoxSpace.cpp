#include "SkyBoxSpace.h"
#include "Texture2D.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "SkyBox.h"
#include "Mesh.h"
#include "LightDirectional.h"
#include "SkyBoxReflectMesh.h"
#include "SampleSet.h"
SkyBoxSpace::SkyBoxSpace(Ambient* ambient)
	:MotionSpace(ambient)
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
	Light * light = new LightDirectional();
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject();
	Mesh * mesh = new Mesh();
	//mesh->GetMaterial().SetImage("Resource/texture/wood.jpg");
	mesh->GetMaterial().color = Color(0.0f, 0.0f, 1.0f);
	meshObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, -0.5f);
	meshObj->AddComponent(mesh);

	GameObject * meshObj2 = new GameObject();
	SkyBoxReflectMesh * mesh2 = new SkyBoxReflectMesh();
	meshObj2->GetComponent(Transform).SetPosition(1.0f, 0.0f, 0.0f);
	meshObj2->AddComponent(mesh2);

	GameObject * meshObj3 = new GameObject();
	SkyBoxReflectMesh * mesh3 = new SkyBoxReflectMesh(
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
	return INSTANCE(Application).Run();
}
#if uSkyBoxSpace
AUTO_APPLICATION_MAIN(SkyBoxSpace)
#endif 