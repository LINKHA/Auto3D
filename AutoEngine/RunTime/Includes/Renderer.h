#pragma once
#include "GameManager.h"
AUTO_BEGIN
class Renderer : public LevelGameManager
{
	REGISTER_DERIVED_CLASS(Renderer, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(Renderer);
public:
	explicit Renderer(Ambient* ambient);
private:
};

AUTO_END