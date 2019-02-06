#pragma once
#include "Texture3D.h"
#include "SkyBoxManager.h"

namespace Auto3D {

class ShaderVariation;

class SkyBox : public Texture3D, public EnableSharedFromThis<SkyBox>
{
	REGISTER_OBJECT_CLASS(SkyBox, Texture3D)
public:
	explicit SkyBox(SharedPtr<Ambient> ambient);
	static void RegisterObject(SharedPtr<Ambient> ambient);

	void Start()override;
	void Draw()override;
	unsigned int GetTexture() { return _cubemapTexture; }
private:
	unsigned int _cubemapTexture;
	SharedPtr<ShaderVariation> _shader;

	unsigned int _skyboxVBO;
};


}

