#pragma once
#include "LevelScene.h"
#include "../FreeCamera.h"

using namespace Auto3D;

class Level_1 : public LevelScene
{
public:
	Level_1(Ambient* ambient, int levelNumber);
	~Level_1() = default;

	void Start()override;
	void Update()override;

	void ShadowPoint();
};

