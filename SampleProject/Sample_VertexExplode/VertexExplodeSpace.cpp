#include "VertexExplodeSpace.h"
#include "GameObject.h"
#include "../FreeCamera.h"
#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include "LightDirectional.h"
#include "Configs.h"
VertexExplodeSpace::VertexExplodeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}


VertexExplodeSpace::~VertexExplodeSpace()
{
}
void VertexExplodeSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject(_ambient);
	Light * light = new LightDirectional(_ambient);
	lightObj->AddComponent(light);

	GameObject * meshObj = new GameObject(_ambient);
	Shader shader(shader_path + "au_vertex_explode.auvs",
		shader_path + "au_vertex_explode.aufs",
		shader_path + "au_vertex_explode.augs");
	Mesh * mesh = new Mesh(_ambient,"../Resource/object/nanosuit/nanosuit.obj",shader);
	meshObj->AddComponent(mesh);

	GameObject * meshObj2 = new GameObject(_ambient);
	Mesh * mesh2 = new Mesh(_ambient,"../Resource/object/nanosuit/nanosuit.obj");
	mesh2->GetMaterial().isTexture = true;
	meshObj2->AddComponent(mesh2);
	meshObj2->GetComponent(Transform).SetPosition(10.0f, 0.0f, 0.0f);
}
void VertexExplodeSpace::Update()
{}
int VertexExplodeSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(VertexExplodeSpace)