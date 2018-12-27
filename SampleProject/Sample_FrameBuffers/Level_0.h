#pragma once
#include "LevelScene.h"

using namespace Auto3D;
class FreeCamera;
class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:
	void Start()override;
	void Update()override;
private:
	int oldi = 0;
	FreeCamera* freeCamera;
};

