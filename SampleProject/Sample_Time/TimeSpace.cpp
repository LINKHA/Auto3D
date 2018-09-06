#include "TimeSpace.h"
#include "Application.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "Time.h"




TimeSpace::TimeSpace(Ambient* ambient)
	:MotionSpace(ambient)
{}
TimeSpace::~TimeSpace()
{}

void TimeSpace::Start()
{
	auto timer = GetSubSystem<Time>();
	//print(timer->GetSmoothDeltaTime());
}

void TimeSpace::Update()
{
	auto timer = GetSubSystem<Time>();
	//print(timer->GetDeltaTime());
	
}

int TimeSpace::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(TimeSpace)