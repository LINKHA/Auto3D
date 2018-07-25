#pragma once
#include "Auto.h"
#include "GameManager.h"
AUTO_BEGIN
/**
* @brief Singleton class
*/
class BaseSpace : public LevelGameManager
{
	REGISTER_DERIVED_CLASS(BaseSpace, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(BaseSpace);
public:
	explicit BaseSpace(Ambient* ambient);
	void Awake();
	void Start(); 
	void Update(); 
	void FixUpdate();
	void Finish();
	void Draw();
};

AUTO_END

