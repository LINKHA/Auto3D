#pragma once
#include "LevelScene.h"
#include "AudioSource.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
public:
	Level_0(Ambient* ambient, int levelNumber);
	~Level_0() = default;
	
	void Start()override;
	void Update()override;

	AudioSource* audio;
	AudioSource* audio2;
};

