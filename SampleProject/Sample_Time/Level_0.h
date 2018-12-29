#pragma once
#include "LevelScene.h"
#include "Time.h"

using namespace Auto3D;

class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:

	void Start()override;
	void Update()override;

	void RealTimes();
	void ScaleTime(float scale);
	void NormalTime();
	void PauseTime();
private:
	SharedPtr<Time> _timer;
	int flag{};
};

