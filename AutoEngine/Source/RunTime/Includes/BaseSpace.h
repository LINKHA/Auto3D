#pragma once
#include "Auto.h"
#include "GameManager.h"
namespace Auto3D {
/**
* @brief Singleton class
*/
class BaseSpace : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(BaseSpace, GlobalGameManager)
public:
	explicit BaseSpace(Ambient* ambient);
	void Init();
	void Destruct();

};

}

