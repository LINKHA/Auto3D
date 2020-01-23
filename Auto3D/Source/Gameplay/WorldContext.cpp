#include "GamePlay/WorldContext.h"
#include "Debug/Log.h"

namespace Auto3D
{

IMPLEMENT_SINGLETON(FWorldContext)

FWorldContext::FWorldContext() :
	_activeWorld(nullptr)
{
}
FWorldContext::~FWorldContext()
{
}

void FWorldContext::SetActiveWorld(SPtr<AWorld> world)
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

SPtr<AWorld> FWorldContext::GetActiveWorld()
{
	return _activeWorld;
}


}