#pragma once
#include "Material.h"
#include "Camera.h"
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
	/// End load reousrce.
	virtual bool EndLoad();
	/// Set up ibl map.
	bool SetupIBL(SkyBox* skybox);
private:
	/// IBL cube map.
	Texture* _iblCubeMap;
};

}