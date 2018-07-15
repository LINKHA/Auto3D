#include "DeferredShadingSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "FreeCamera.h"
#include "Mesh.h"
#include "DeferredShading.h"

DeferredShadingSpace::DeferredShadingSpace()
{
}


DeferredShadingSpace::~DeferredShadingSpace()
{
}
void DeferredShadingSpace::Start()
{
	GameObject * cameraObj = new GameObject();
	FreeCamera * camera = new FreeCamera();
	cameraObj->GetComponent(Transform).SetPosition(0.0f, 0.0f, 3.0f);
	cameraObj->AddComponent(camera);


	GameObject * meshObj = new GameObject();
	DeferredShading * deferred = new DeferredShading();
	meshObj->AddComponent(deferred);
}

void DeferredShadingSpace::Update()
{
}

int DeferredShadingSpace::Launch()
{
	return INSTANCE(Application).Run();
}
