#pragma once
#include "MotionSpace.h"
#include "Time.h"
using namespace Auto3D;
class FreeCamera;

class TimeSpace : public MotionSpace
{
public:
	explicit TimeSpace(Ambient* ambient);
	~TimeSpace();
	void Awake()override;
	//void Start()override;
	//void Update()override;
	int Launch();
//
//	void RealTimes();
//	void ScaleTime(float scale);
//	void NormalTime();
//	void PauseTime();
//private:
//	Auto3D::Time* _timer;
//	int flag{};
};


