#pragma once
#include "Texture3D.h"
#include "SkyBoxManager.h"

namespace Auto3D {

class SkyBox : public Texture3D
{
	REGISTER_DERIVED_CLASS(SkyBox, Texture3D);
	DECLARE_OBJECT_SERIALIZE(SkyBox);
public:
	explicit SkyBox(Ambient* ambient);
	void Start()override;
	void Draw()override;
	unsigned int GetTexture() { return _cubemapTexture; }
private:
	unsigned int _cubemapTexture;
	_Shader _shader;
	unsigned int _skyboxVAO;
	unsigned int _skyboxVBO;
};


}

