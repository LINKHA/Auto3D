#pragma once
#include "LevelScene.h"
#include "PhysicsWorld2D.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:

	void Awake()override;
	void Start()override;
	void Update()override;

	GameNode groundNode;
	GameNode bodyNode;
};

