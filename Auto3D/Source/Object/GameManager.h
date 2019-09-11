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

class AUTO_API BaseModule : public GameManager
{
	REGISTER_OBJECT_CLASS(BaseModule, GameManager)
public:
	BaseModule() = default;
	~BaseModule() = default;
};

}