#include "NormalMappingSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "TextureNormal.h"
#include "SampleSet.h"
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
	TextureNormal * normal = new TextureNormal();
	normalObj->AddComponent(normal);
	

}

void NormalMappingSpace::Update()
{
}

int NormalMappingSpace::Launch()
{
	return INSTANCE(Application).Run();
}
#if uNormalMappingSpace
AUTO_APPLICATION_MAIN(NormalMappingSpace)
#endif