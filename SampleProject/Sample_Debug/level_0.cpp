#include "Level_0.h"
#include "GameObject.h"
#include "LightDirectional.h"
#include "Mesh.h"
#include "../FreeCamera.h"
#include "SkyBox.h"
#include "Time.h"
#include "Timer.h"

Level_0::Level_0(Ambient* ambient, int levelNumber)
	:LevelScene(ambient, levelNumber)
{}

void callBack()
{
	AutoCout << "1111111111"<< AutoCoutEnd;
}
void Level_0::Start()
{
	//GetSubSystem<Time>()->ShotTimer(callBack, 1000,5);
	//GetSubSystem<Time>()->OneShotTimer(callBack, 1000);

	Timer timer(callBack, 1000);
	//GetSubSystem<Time>()->ShotTimer(callBack, 1000);
	//Print(1);
	//GetSubSystem<Time>()->OneShotTimer(1000, callBack);
	//Print(2);
}

void Level_0::Update()
{
}


