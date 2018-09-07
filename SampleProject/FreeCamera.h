#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "ObjectDefines.h"

using namespace Auto3D;
class FreeCamera : public ScriptComponent
{
public:
	FreeCamera(Ambient* ambient,int levelNumber);
	~FreeCamera();
	void Start()override;
	void Update()override;
	GameObject* freeCameraObject;
	SharedPtr<Camera> freeCamera;

private:
	void processInput();
};

