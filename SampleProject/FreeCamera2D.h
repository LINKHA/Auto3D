#pragma once
#include "ScriptComponent.h"
#include "Camera.h"

using namespace Auto3D;

class FreeCamera2D : public ScriptComponent
{
	REGISTER_SCRIPT_CLASS(FreeCamera2D)
public:
	void Init()override;
	void Start()override;
	void Update()override;

	SharedPtr<Node> cameraNode;
	SharedPtr<Camera> camera;
	float speed = 2.5f;
private:
	void processInput();
};