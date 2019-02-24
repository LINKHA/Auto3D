#pragma once
#include "GameManager.h"
#include "GraphicsDef.h"
#include "Image.h"
#include "Batch.h"

namespace Auto3D {


static const int INSTANCING_BUFFER_DEFAULT_SIZE = 1024;

const static char* RESOURCE_ATTACH = "Resource_attach";

/// Description of a pass from the client to the renderer.
struct PassDesc
{
	/// Construct undefined.
	PassDesc()
	{
	}

	/// Construct with parameters.
	PassDesc(const STRING& name_, BatchSortMode sort_ = SORT_STATE, bool lit_ = true) :
		name(name_),
		sort(sort_),
		lit(lit_)
	{
	}

	/// %Pass name.
	STRING name;
	/// Sorting mode.
	BatchSortMode sort;
	/// Lighting flag.
	bool lit;
};

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