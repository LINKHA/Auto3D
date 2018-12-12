#pragma once
#include "GameManager.h"

namespace Auto3D {

class UI : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(UI, GlobalGameManager)
public:
	explicit UI(Ambient* ambient);

};


}