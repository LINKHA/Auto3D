#pragma once
#include "Core/Object.h"

namespace Auto3D
{

class AUTO_API AGameManager : public AObject
{
	REGISTER_OBJECT_CLASS(AGameManager, AObject)

public:
	AGameManager() = default;
	virtual ~AGameManager() = default;
};

class AUTO_API ABaseModule : public AGameManager
{
	REGISTER_OBJECT_CLASS(ABaseModule, AGameManager)
public:
	ABaseModule() = default;
	virtual ~ABaseModule() = default;
};

}