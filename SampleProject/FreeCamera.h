#pragma once
#include "ScriptComponent.h"
#include "Camera.h"

using namespace Auto3D;

class FreeCamera : public ScriptComponent
{
	REGISTER_SCRIPT_CLASS(FreeCamera)
public:
	void Init()override;
	void Start()override;
	void Update()override;

	Node* cameraNode;
	Camera* camera;
private:
	void processInput();
};