#include "GamePlay/WorldContext.h"
#include "Debug/Log.h"

namespace Auto3D
{

IMPLEMENT_SINGLETON(GWorldContext)

GWorldContext::GWorldContext() :
	_activeWorld(nullptr),
	_activeScene2d(nullptr)
{
}
GWorldContext::~GWorldContext()
{
}

void GWorldContext::SetActiveWorld(AWorld* world)
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

void GWorldContext::SetActiveScene2D(AScene2D* scene2d)
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

AWorld* GWorldContext::GetActiveWorld()
{
	return _activeWorld;
}

AScene2D* GWorldContext::GetActiveScene2D()
{
	return _activeScene2d;
}

}