#include "GamePlay/WorldContext.h"
#include "Debug/Log.h"
#include "Gameplay/World.h"

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

void FWorldContext::SetActiveWorld(AWorld* world)
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

void FWorldContext::DeleteWorld(AWorld* world)
{
	if (world)
	{
		auto it = _worlds.Find(world);
		if (it != _worlds.End())
		{
			_worlds.Erase(it);

			world->RemoveSelf();
		}
	}
}

void FWorldContext::DeleteAllWorld()
{
	for (auto it = _worlds.Begin(); it != _worlds.End(); ++it)
	{
		(*it)->RemoveSelf();
	}
	_worlds.Clear();
}

AWorld* FWorldContext::GetActiveWorld()
{
	return _activeWorld;
}

AWorld* FWorldContext::NewWorld()
{
	AWorld* world = new AWorld();
	RegisterWorld(world);
	return world;
}

}