#include "Level_0.h"
#include "Input.h"
#include "Node.h"


void Level_0::Start()
{
	_timer = GetSubSystem<Time>();
}

void Level_0::Update()
{
	if (GetSubSystem<Input>()->GetKeyDown(KEY_SPACE))
	{
		flag++;
		if (flag >= 4)
			flag = 0;
	}
	if (GetSubSystem<Input>()->GetKeyDown(KEY_S))
	{
		_timer->Sleep(3000);
	}
	switch (flag)
	{
	case 0: RealTimes(); break;
	case 1: ScaleTime(0.5f); break;
	case 2: NormalTime(); break;
	case 3: PauseTime(); break;
	default:
		break;
	}
}

void Level_0::RealTimes()
{
	RealTime& time = _timer->GetRealTime();
	AutoCout << "year : " << time.year << "  "
		<< "month : " << time.month << "  "
		<< "day : " << time.day << "  "
		<< "hour : " << time.hour << "  "
		<< "minute : " << time.minute << "  "
		<< "second : " << time.second
		<< AutoEndl;
}
void Level_0::ScaleTime(float scale)
{
	_timer->SetTimeScale(scale);
	_timer->SetPause(false);
	AutoCout << _timer->GetCurTime() << "  "
		<< _timer->GetTimeSinceStartup() << "  "
		<< _timer->GetDeltaTime() << "  "
		<< _timer->GetSmoothDeltaTime() << "  "
		<< _timer->GetFrameCount()
		<< AutoEndl;

}
void Level_0::NormalTime()
{
	_timer->SetTimeScale(1.0f);
	_timer->SetPause(false);
	AutoCout << _timer->GetCurTime() << "  "
		<< _timer->GetTimeSinceStartup() << "  "
		<< _timer->GetDeltaTime() << "  "
		<< _timer->GetSmoothDeltaTime() << "  "
		<< _timer->GetFrameCount()
		<< AutoEndl;
}
void Level_0::PauseTime()
{
	_timer->SetTimeScale(1.0f);
	_timer->SetPause(true);
	AutoCout << _timer->GetCurTime() << "  "
		<< _timer->GetTimeSinceStartup() << "  "
		<< _timer->GetDeltaTime() << "  "
		<< _timer->GetSmoothDeltaTime() << "  "
		<< _timer->GetFrameCount()
		<< AutoEndl;
}

