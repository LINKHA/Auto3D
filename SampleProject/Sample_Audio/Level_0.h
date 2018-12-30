#pragma once
#include "LevelScene.h"
#include "AudioSource.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:
	void Start()override;
	void Update()override;

	SharedPtr<AudioSource> audio;
	SharedPtr<AudioSource> audio2;
};

