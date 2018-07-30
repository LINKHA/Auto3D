#include "DeferredShadingSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Mesh.h"
#include "DeferredShading.h"
DeferredShadingSpace::DeferredShadingSpace(Ambient* ambient)
	:MotionSpace(ambient)
{
}


DeferredShadingSpace::~DeferredShadingSpace()
{
}
void DeferredShadingSpace::Start()
{
	GameObject * cameraObj = new GameObject(_ambient);
	FreeCamera * camera = new FreeCamera(_ambient);
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject * meshObj = new GameObject(_ambient);
	DeferredShading * deferred = new DeferredShading(_ambient);
	meshObj->AddComponent(deferred);
}

void DeferredShadingSpace::Update()
{
}

int DeferredShadingSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(DeferredShadingSpace)