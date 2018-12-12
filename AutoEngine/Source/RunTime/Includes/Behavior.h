#pragma once
#include "GameManager.h"

namespace Auto3D {

class Behavior : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Behavior, GlobalGameManager)
public:
	explicit Behavior(Ambient* ambient);
	void Awake();
	void Start();
	void Update();
	void FixUpdate();
	void Finish();
	void Draw();
};

}