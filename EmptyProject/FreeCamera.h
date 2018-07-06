#pragma once
#include "ScriptComponent.h"
#include "GameObject.h"
#include "Camera.h"
USING_AUTO

class FreeCamera : public ScriptComponent
{
public:
	FreeCamera();
	~FreeCamera();
	void Start()override;
	void Update()override;
	
};

