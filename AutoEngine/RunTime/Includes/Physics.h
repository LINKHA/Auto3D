#pragma once
#include "GameManager.h"
namespace Auto3D {

class Physics : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Physics, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Physics);
public:
	explicit Physics(Ambient* ambient);


};

}