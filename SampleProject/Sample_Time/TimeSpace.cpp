#include "TimeSpace.h"
#include "Application.h"
#include "Input.h"
#include "Level_0.h"


TimeSpace::TimeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
TimeSpace::~TimeSpace()
{}
void TimeSpace::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}
//
//void TimeSpace::Start()
//{
//	_timer = GetSubSystem<Time>();
//}
//
//void TimeSpace::Update()
//{
//	if (GetSubSystem<Input>()->GetKeyDown(KEY_SPACE))
//	{
//		flag++;
//		if (flag >= 4)
//			flag = 0;
//	}
//	if (GetSubSystem<Input>()->GetKeyDown(KEY_S))
//	{
//		_timer->Sleep(3000);
//	}
//	switch (flag)
//	{
//	case 0: RealTimes(); break;
//	case 1: ScaleTime(0.5f); break;
//	case 2: NormalTime(); break;
//	case 3: PauseTime(); break;
//	default:
//		break;
//	}
//}
//
//void TimeSpace::RealTimes()
//{
//	RealTime& time = _timer->GetRealTime();
//	AutoCout <<"year : " <<time.year << "  "
//		<< "month : " << time.month << "  "
//		<< "day : " << time.day << "  "
//		<< "hour : " << time.hour << "  "
//		<< "minute : " << time.minute << "  "
//		<< "second : " << time.second
//		<< AutoCoutEnd;
//}
//void TimeSpace::ScaleTime(float scale)
//{
//	_timer->SetTimeScale(scale);
//	_timer->SetPause(false);
//	AutoCout << _timer->GetCurTime() << "  " 
//		<< _timer->GetTimeSinceStartup()<< "  " 
//		<< _timer->GetDeltaTime() << "  " 
//		<< _timer->GetSmoothDeltaTime()<< "  " 
//		<< _timer->GetFrameCount() 
//		<< AutoCoutEnd;
//
//}
//void TimeSpace::NormalTime()
//{
//	_timer->SetTimeScale(1.0f);
//	_timer->SetPause(false);
//	AutoCout << _timer->GetCurTime() << "  "
//		<< _timer->GetTimeSinceStartup() << "  "
//		<< _timer->GetDeltaTime() << "  "
//		<< _timer->GetSmoothDeltaTime() << "  "
//		<< _timer->GetFrameCount()
//		<< AutoCoutEnd;
//}
//void TimeSpace::PauseTime()
//{
//	_timer->SetTimeScale(1.0f);
//	_timer->SetPause(true);
//	AutoCout << _timer->GetCurTime() << "  "
//		<< _timer->GetTimeSinceStartup() << "  "
//		<< _timer->GetDeltaTime() << "  "
//		<< _timer->GetSmoothDeltaTime() << "  "
//		<< _timer->GetFrameCount()
//		<< AutoCoutEnd;
//}

int TimeSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(TimeSpace)