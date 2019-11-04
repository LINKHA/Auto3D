#pragma once
#include "Material.h"
#include "../Renderer/Camera.h"
namespace Auto3D
{

class IBLMaterial : public Material 
{
	REGISTER_OBJECT_CLASS(IBLMaterial, Material)

public:
	/// Construct.
	IBLMaterial();
	/// Destruct.
	virtual ~IBLMaterial();

	/// Register object factory.
	static void RegisterObject();

	void CreatePass(Camera* camera);

	void SetIrradiance();

	void SetPrefilter();

	void SetBrdfLut();

	void SetAAA(Texture* iblCube);
};

}