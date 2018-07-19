#pragma once
#include "Texture3D.h"
#include "Singleton.h"
AUTO_BEGIN

class SkyBox : public Texture3D
{
	REGISTER_DERIVED_CLASS(SkyBox, Texture3D);
	DECLARE_OBJECT_SERIALIZE(SkyBox);
public:
	SkyBox();

	void Start()override;
	void Draw()override;
	unsigned int GetTexture() { return m_cubemapTexture; }
private:
	unsigned int m_cubemapTexture;

	Shader m_shader;
	unsigned int m_skyboxVAO;
	unsigned int m_skyboxVBO;
};

struct SkyManager : public Singleton<SkyManager>
{
	void AddSkyBox(SkyBox* skybox) { m_skybox = skybox; }
	SkyBox * GetSkyBox() { return m_skybox; }
private:
	SkyBox * m_skybox;
};
AUTO_END

