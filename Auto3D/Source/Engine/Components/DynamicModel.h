#pragma once

#include "Scene/GeometryNode.h"

namespace Auto3D
{

class AModel;
/// %Scene node that renders an animated model.
class AUTO_API ADynamicModel : public AGeometryNode
{
	REGISTER_OBJECT_CLASS(ADynamicModel, AGeometryNode)

public:
	/// Construct.
	ADynamicModel();
	/// Destruct.
	~ADynamicModel();

	/// Register factory and attributes.
	static void RegisterObject();

	/// Prepare object for rendering. Reset framenumber and light list and calculate distance from camera.
	//void OnPrepareRender(unsigned frameNumber, ACamera* camera) override;

	//void SetWaterData();

	bool init();
	void reshape(unsigned width, unsigned height);
	void renderWater(float passedTime);
	bool update(float time);


	void AAA();
	void BBB();
};

}
