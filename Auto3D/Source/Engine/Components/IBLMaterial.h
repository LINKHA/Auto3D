#pragma once
#include "Material.h"
#include "Camera.h"
namespace Auto3D
{
class ASkyBox;

class AIBLMaterial : public AMaterial 
{
	DECLARE_CLASS(AIBLMaterial, AMaterial)

public:
	/// Construct.
	AIBLMaterial();
	/// Destruct.
	virtual ~AIBLMaterial();

	/// Register object factory.
	static void RegisterObject();
	/// End load reousrce.
	virtual bool EndLoad();
	/// Set up ibl map.
	bool SetupIBL(ASkyBox* skybox);
private:
	/// IBL cube map.
	ATexture* _iblCubeMap;
};

}