#pragma once
#include "GameManager.h"
namespace Auto3D {

class Physics : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Physics, GlobalGameManager)
public:
	explicit Physics(Ambient* ambient);


};

}