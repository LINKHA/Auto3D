#pragma once

#include "Renderer/GeometryNode.h"

namespace Auto3D
{

class Model;
/// %Scene node that renders an animated model.
class AUTO_API DynamicModel : public GeometryNode
{
	REGISTER_OBJECT_CLASS(DynamicModel, GeometryNode)

public:
	/// Construct.
	DynamicModel();
	/// Destruct.
	~DynamicModel();

	/// Register factory and attributes.
	static void RegisterObject();

	/// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera.
	//void OnPrepareRender(unsigned frameNumber, Camera* camera) override;

	//void SetWaterData();

	bool init();
	void reshape(unsigned width, unsigned height);
	void renderWater(float passedTime);
	bool update(float time);


	void AAA();
	void BBB();
};

}
