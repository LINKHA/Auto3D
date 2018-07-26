#pragma once
#include "Auto.h"
#include "stl_use.h"
#include "ManagerTool.h"
AUTO_BEGIN
class SpriteTranslucent;
class SpriteSort : public ManagerTool
{
	REGISTER_DERIVED_CLASS(SpriteSort, ManagerTool);
	DECLARE_OBJECT_SERIALIZE(SpriteSort);
public:
	explicit SpriteSort(Ambient* ambient);
	void AddSprite(SpriteTranslucent * sprite);
	void ComputeMap();
	void RenderSprite();
private:
	_VECTOR(SpriteTranslucent*) _sprites;
	AUTO_MAP(float, SpriteTranslucent*) _sorted;
};
AUTO_END