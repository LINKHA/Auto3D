#pragma once
#include "LevelScene.h"
#include "Physics2DWorld.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
public:
	Level_0(Ambient* ambient, int levelNumber);
	~Level_0() = default;

	void Start()override;
	void Update()override;
	Physics2DWorld* physicsWorld;
	b2Body* body;
};

