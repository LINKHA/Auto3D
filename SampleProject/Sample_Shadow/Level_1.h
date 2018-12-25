#pragma once
#include "LevelScene.h"


using namespace Auto3D;

class Level_1 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_1)
public:

	void Start()override;
	void Update()override;

	void ShadowPoint();
};

