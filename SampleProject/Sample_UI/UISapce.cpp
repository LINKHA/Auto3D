#include "UISapce.h"
#include "Application.h"
#include "../FreeCamera.h"
#include "level_0.h"
UISapce::UISapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
UISapce::~UISapce()
{
	RemoveLevel(0);
}
void UISapce::Awake()
{
	RegisterLevel(new Level_0(_ambient, 0));
}

void UISapce::Start()
{
}
void UISapce::Update()
{
}
int UISapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(UISapce)