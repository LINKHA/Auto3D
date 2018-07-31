#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
#include "Camera.h"
using namespace Auto3D;

class FreeCamera : public ScriptComponent
{
public:
	FreeCamera();
	~FreeCamera();
	void Start()override;
	void Update()override;
	
};

