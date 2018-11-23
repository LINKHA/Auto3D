#pragma once
#include "LevelScene.h"


using namespace Auto3D;

class Level_0 : public LevelScene
{
public:
	Level_0(Ambient* ambient, int id);
	~Level_0() = default;
	void Awake()override;
	void Start()override;
	void Update()override;


};

