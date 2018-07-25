#pragma once
#include "Singleton.h"
#include "Auto.h"
#include "stl_use.h"
AUTO_BEGIN
class SpriteTranslucent;
class SpriteSort : public Singleton<SpriteSort>
{
public:
	SpriteSort();
	~SpriteSort();
	void AddSprite(SpriteTranslucent * sprite);
	void ComputeMap();
	void RenderSprite();
private:
	_VECTOR(SpriteTranslucent*) _sprites;
	AUTO_MAP(float, SpriteTranslucent*) _sorted;
};
AUTO_END