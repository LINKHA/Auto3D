#pragma once
//#include "ScriptComponent.h"
//#include "Camera.h"
//
//using namespace Auto3D;
//
//class FreeCamera : public ScriptComponent
//{
//	REGISTER_SCRIPT_CLASS(FreeCamera)
//public:
//	void Init()override;
//	void Start()override;
//	void Update()override;
//
//	SharedPtr<Node> cameraNode;
//	SharedPtr<Camera> camera;
//
//	float speed = 5.0f;
//	float mouseSensitivity = 0.1f;
//	Vector2 limitZoom{ Vector2(1,45) };
//private:
//	void processInput();
//	void handleMouseWheelOffset();
//};