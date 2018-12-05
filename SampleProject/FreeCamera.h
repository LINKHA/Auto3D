#pragma once
#include "ScriptComponent.h"
#include "Camera.h"

using namespace Auto3D;
class FreeCamera : public ScriptComponent
{
public:
	FreeCamera(Ambient* ambient,int levelNumber);
	~FreeCamera();
	void Start()override;
	void Update()override;
	Node* freeCameraObject;
	Camera* freeCamera;
private:
	void processInput();
};