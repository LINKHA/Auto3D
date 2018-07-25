#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "ObjectDefines.h"

USING_AUTO
class FreeCamera : public ScriptComponent
{
public:
	FreeCamera(Ambient* ambient);
	~FreeCamera();
	void Start()override;
	void Update()override;
	GameObject* freeCameraObject;
	Camera * freeCamera;
private:
	void processInput();
};

