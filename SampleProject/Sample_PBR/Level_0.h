#pragma once
#include "LevelScene.h"
#include "Mesh.h"

using namespace Auto3D;
#define USE_DEMO 0
class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:

	void Start()override;
	void Update()override;
private:
	void wood(float offset);
	void init();
	void back();

	void demo();
	
	Mesh* cube;
};