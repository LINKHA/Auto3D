#include "ParallaxMappingSpace.h"
#include "Application.h"
#include "Light.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "TextureParallax.h"
ParallaxMappingSpace::ParallaxMappingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
ParallaxMappingSpace::~ParallaxMappingSpace()
{}

void ParallaxMappingSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);

	/*GameObject * lightObj = new GameObject(_ambient);
	Light * light = new Light(Directional);
	lightObj->AddComponent(light);*/

	GameObject * normalObj = new GameObject(_ambient);
	TextureParallax * normal = new TextureParallax(_ambient);
	normalObj->AddComponent(normal);


}

void ParallaxMappingSpace::Update()
{
}

int ParallaxMappingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(ParallaxMappingSpace)