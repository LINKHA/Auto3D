#include "VertexExplodeSpace.h"
#include "GameObject.h"
#include "FreeCamera.h"
#include "Application.h"
#include "Mesh.h"
#include "Shader.h"
#include "LightPoint.h"
#include "SampleSet.h"
VertexExplodeSpace::VertexExplodeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}


VertexExplodeSpace::~VertexExplodeSpace()
{
}
void VertexExplodeSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	GameObject * lightObj = new GameObject();
	Light * light = new LightPoint();
	lightObj->AddComponent(light);


	GameObject * meshObj = new GameObject();
	Shader shader(AtConfig::shader_path + "au_vertex_explode.auvs",
		AtConfig::shader_path + "au_vertex_explode.aufs",
		AtConfig::shader_path + "au_vertex_explode.augs");

	Mesh * mesh = new Mesh("Resource/object/nanosuit/nanosuit.obj",shader);
	meshObj->AddComponent(mesh);

	GameObject * meshObj2 = new GameObject();
	Mesh * mesh2 = new Mesh("Resource/object/base/Cube.FBX");
	meshObj->AddComponent(mesh2);
}
void VertexExplodeSpace::Update()
{}
int VertexExplodeSpace::Launch()
{
	return INSTANCE(Application).Run();
}
#if uVertexExplodeSpace
AUTO_APPLICATION_MAIN(VertexExplodeSpace)
#endif