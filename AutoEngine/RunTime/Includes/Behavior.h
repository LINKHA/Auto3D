#pragma once
#include "GameManager.h"

namespace Auto3D {

class Behavior : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Behavior, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Behavior);
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