#pragma once
#include "LevelScene.h"
#include "Node.h"


using namespace Auto3D;

class Level_0 : public LevelScene
{
	REGISTER_LEVEL_CLASS(Level_0)
public:

	void Awake()override;
	void Start()override;
	void Update()override;
public:
	SharedPtr<Node> obj;

	SharedPtr<Node> camObj;
	SharedPtr<Node> obj5;
	SharedPtr<Node> obj2;

};

