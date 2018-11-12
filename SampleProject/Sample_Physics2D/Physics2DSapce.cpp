#include "Physics2DSapce.h"
#include "Application.h"
#include "LightDirectional.h"
#include "../FreeCamera.h"
#include "Level_0.h"
Physics2DSapce::Physics2DSapce(Ambient* ambient)
	:MotionSpace(ambient)
{}
Physics2DSapce::~Physics2DSapce()
{
	RemoveScene(0);
}

void Physics2DSapce::Awake()
{
	RegisterScene(new Level_0(_ambient, 0));
}
void Physics2DSapce::Update()
{
}
int Physics2DSapce::Launch()
{
	return INSTANCE(Application).Run(_ambient);
}
AUTO_APPLICATION_MAIN(Physics2DSapce)