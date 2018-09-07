#pragma once
#include "LevelScene.h"
#include "Time.h"
using namespace Auto3D;


class Level_0 : public LevelScene
{
public:
	explicit Level_0(Ambient* ambient,int levelNumber);
	~Level_0() = default;

	void Start()override;
	void Update()override;

	void RealTimes();
	void ScaleTime(float scale);
	void NormalTime();
	void PauseTime();
private:
	Auto3D::Time* _timer;
	int flag{};
};

