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

class AUTO_API Subsystem : public GameManager
{
	REGISTER_OBJECT_CLASS(Subsystem, Object)
public:
	Subsystem() = default;
	~Subsystem() = default;
};



}