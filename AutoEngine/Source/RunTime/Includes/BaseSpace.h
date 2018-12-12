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
	void Awake();
	void Start(); 
	void Update(); 
	void FixUpdate();
	void Finish();
	void Draw();
};

}

