#include "RegisteredBox.h"
#include "Debug/Log.h"

namespace Auto3D
{

FRegisteredBoxModule::FRegisteredBoxModule() :
	_activeWorld(nullptr),
	_activeScene2d(nullptr)
{
}
FRegisteredBoxModule::~FRegisteredBoxModule()
{
}

void FRegisteredBoxModule::SetActiveWorld(AWorld* world)
{ 
	if (world)
	{
		_activeWorld = world;
	}
	else
	{
		WarningString("File set active world.");
	}
}

void FRegisteredBoxModule::SetActiveScene2D(AScene2D* scene2d)
{
	if (scene2d)
	{
		_activeScene2d = scene2d;
	}
	else
	{
		WarningString("File set active scene 2D.");
	}
}

AWorld* FRegisteredBoxModule::GetActiveWorld()
{
	return _activeWorld;
}

AScene2D* FRegisteredBoxModule::GetActiveScene2D()
{
	return _activeScene2d;
}

}