#pragma once
#include "Core/Object.h"

namespace Auto3D
{

class AUTO_API GameManager : public AObject
{
	REGISTER_OBJECT_CLASS(GameManager, AObject)

public:
	GameManager() = default;
	virtual ~GameManager() = default;
};

class AUTO_API ABaseModule : public GameManager
{
	REGISTER_OBJECT_CLASS(ABaseModule, GameManager)
public:
	ABaseModule() = default;
	virtual ~ABaseModule() = default;
};

}