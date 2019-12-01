#include "RegisteredBox.h"
#include "Debug/Log.h"

namespace Auto3D
{

FRegisteredBoxModule::FRegisteredBoxModule() :
	_activeScene(nullptr),
	_activeScene2d(nullptr)
{
}
FRegisteredBoxModule::~FRegisteredBoxModule()
{
}

void FRegisteredBoxModule::SetActiveScene(AScene* scene)
{ 
	if (scene)
	{
		_activeScene = scene;
	}
	else
	{
		WarningString("File set active scene.");
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

AScene* FRegisteredBoxModule::GetActiveScene()
{
	return _activeScene;
}

AScene2D* FRegisteredBoxModule::GetActiveScene2D()
{
	return _activeScene2d;
}

}