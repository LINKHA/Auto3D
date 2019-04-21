#pragma once
#include "StaticModel.h"

namespace Auto3D
{

// Relative to the three - dimensional space independent skybox
class AUTO_API SkyBox : public StaticModel
{
	REGISTER_OBJECT_CLASS(SkyBox, StaticModel)
public:
	/// Construct
	SkyBox();
	/// Destruct
	~SkyBox();
	/// Register factory and attributes.
	static void RegisterObject();
private:
	/// Recalculate the world-space bounding box.
	void OnWorldBoundingBoxUpdate();
};

}

