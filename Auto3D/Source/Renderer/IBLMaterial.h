#pragma once
#include "Material.h"
#include "../Renderer/Camera.h"
namespace Auto3D
{
class SkyBox;

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

	virtual bool EndLoad();

	void SetMapSize(int mapSize) { _mapSize = mapSize; }

	void SetIrradianceSize(int irradianceSize) { _irradianceSize = irradianceSize; }

	void SetupIBL(SkyBox* skybox);
private:

	/// Create an irradiance cubemap, and re-scale capture FBO to irradiance scale.
	SharedPtr<Texture> SetupIrradianceMap();
	/// Create a pre-filter cubemap, and re-scale capture FBO to pre-filter scale.
	SharedPtr<Texture> SetupPrefilterMap();
	/// Generate a 2D LUT from the BRDF equations used.
	SharedPtr<Texture> SetupBrdfLUT();

	int _mapSize;

	int _irradianceSize;

	int _prefilterSize;

	Texture* _iblCubeMap;

	SharedPtr<Texture> _irradianceMap;

	SharedPtr<Texture> _prefilterMap;

	SharedPtr<Texture> _brdfLUT;
};

}