#pragma once
#pragma once
#include "LevelScene.h"
#include "../FreeCamera.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:
	void Start()override;
	void Update()override;
private:
	int oldi = 0;
	FreeCamera* camera;
};

