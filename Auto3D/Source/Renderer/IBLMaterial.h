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

	void CreatePass(Camera* camera);

	void SetIrradiance();

	void SetPrefilter();

	void SetBrdfLut();

	void SetAAA(Texture* iblCube);

	void SetupIBL(SkyBox* skybox);
private:
	int _mapSize;

	int _irradianceSize;

	Texture* _iblCubeMap;
};

}