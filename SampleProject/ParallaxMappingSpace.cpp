#include "ParallaxMappingSpace.h"
#include "Application.h"
#include "Light.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "TextureMap.h"
ParallaxMappingSpace::ParallaxMappingSpace()
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{}

void ParallaxMappingSpace::Start()
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
	normal->useParallax = true;
	normalObj->AddComponent(normal);


}

void ParallaxMappingSpace::Update()
{
}

int ParallaxMappingSpace::Launch()
{
	return INSTANCE(Application).Run();
}
