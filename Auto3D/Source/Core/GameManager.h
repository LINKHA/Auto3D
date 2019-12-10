#pragma once
#include "Core/Object/Object.h"

namespace Auto3D
{

class AUTO_API AGameManager : public AObject
{
	DECLARE_CLASS(AGameManager, AObject)

public:
	AGameManager() = default;
	virtual ~AGameManager() = default;
};

class AUTO_API ABaseModule : public AGameManager
{
	DECLARE_CLASS(ABaseModule, AGameManager)
public:
	ABaseModule() = default;
	virtual ~ABaseModule() = default;
};

}