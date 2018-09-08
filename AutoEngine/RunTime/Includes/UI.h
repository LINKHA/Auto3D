#pragma once
#include "GameManager.h"

namespace Auto3D {

class UI : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(UI, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(UI);
public:
	explicit UI(Ambient* ambient);

};


}