#pragma once
#include "GameManager.h"
#include "GraphicsDef.h"
#include "Image.h"
#include "Batch.h"

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
	void SetupShadowMaps(size_t num, int size, ImageFormat format);
	void Init();
private:
	/// Shadow maps.
	VECTOR<SharedPtr<ShadowMap>> _shadowMaps;
};

}