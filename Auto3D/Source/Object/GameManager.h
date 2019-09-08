#pragma once
#include "Object.h"

namespace Auto3D
{

class AUTO_API GameManager : public Object
{
	REGISTER_OBJECT_CLASS(GameManager, Object)

public:
	GameManager() = default;
	~GameManager() = default;
};

class AUTO_API BaseSubsystem : public GameManager
{
	REGISTER_OBJECT_CLASS(BaseSubsystem, GameManager)
public:
	BaseSubsystem() = default;
	~BaseSubsystem() = default;
};

}