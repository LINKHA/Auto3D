#pragma once
#include "ScriptComponent.h"
#include "Camera.h"
#include "ObjectDef.h"

using namespace Auto3D;
class FreeCamera : public ScriptComponent
{
public:
	FreeCamera(Ambient* ambient, int levelNumber);
	~FreeCamera();
	void Start()override;
	void Update()override;
	GameObject* freeCameraObject;
	Camera* freeCamera;
private:
	void processInput();
};

