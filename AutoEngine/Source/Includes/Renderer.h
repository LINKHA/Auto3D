#pragma once
#include "GameManager.h"
#include "GraphicsDef.h"

namespace Auto3D {

static const int INSTANCING_BUFFER_DEFAULT_SIZE = 1024;

/**
* @brief : Render graphices create to geometry
*/
class Renderer : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Renderer, GlobalGameManager)
public:
	explicit Renderer(SharedPtr<Ambient> ambient);
	
};

}