#pragma once
#include "LevelScene.h"

using namespace Auto3D;
#define USE_DEMO 1
class Level_0 : public LevelScene
{
public:
	Level_0(Ambient* ambient, int levelNumber);
	~Level_0() = default;

	void Start()override;
	void Update()override;
private:
	void wood(float offset);
	void init();
	void back();

	void demo();

};