#include "NormalMappingSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "TextureMap.h"
NormalMappingSpace::NormalMappingSpace()
{}
NormalMappingSpace::~NormalMappingSpace()
{}

void NormalMappingSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	/*GameObject * lightObj = new GameObject();
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);*/

	GameObject * normalObj = new GameObject();
	TextureMap * normal = new TextureMap();
	normalObj->AddComponent(normal);
	

}

void NormalMappingSpace::Update()
{
}

int NormalMappingSpace::Launch()
{
	return INSTANCE(Application).Run();
}
