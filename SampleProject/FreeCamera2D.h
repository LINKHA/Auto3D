#pragma once
#include "ScriptComponent.h"
#include "tCamera.h"

using namespace Auto3D;

class FreeCamera2D : public ScriptComponent
{
	REGISTER_SCRIPT_CLASS(FreeCamera2D)
public:
	void Init()override;
	void Start()override;
	void Update()override;

	SharedPtr<Node> cameraNode;
	SharedPtr<tCamera> camera;
	float speed = 5.0f;
private:
	void processInput();
};