#pragma once
#include "Texture3D.h"

AUTO_BEGIN

class SkyBox : public Texture3D
{
	REGISTER_DERIVED_CLASS(SkyBox, Texture3D);
	DECLARE_OBJECT_SERIALIZE(SkyBox);
public:
	SkyBox();

	void Start()override;
	void Draw()override;
private:
	unsigned int m_cubemapTexture;

	Shader m_shader;
	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;
};

AUTO_END

