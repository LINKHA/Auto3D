#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
#include "Camera.h"
#include "ObjectDefines.h"

USING_AUTO
class Ambient;
class FreeCamera : public ScriptComponent
{
public:
	FreeCamera(AUTO::Ambient* ambient);
	~FreeCamera();
	void Start()override;
	void Update()override;
	GameObject* freeCameraObject;
	Camera * freeCamera;
private:
	void processInput();
	AUTO::Ambient* _ambient;
};

