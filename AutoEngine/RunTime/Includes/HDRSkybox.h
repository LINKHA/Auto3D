#pragma once
#include "Texture3D.h"
#include "Singleton.h"
AUTO_BEGIN

class HDRSkyBox : public Texture3D
{
public:
	HDRSkyBox();
	~HDRSkyBox();
	void Start()override;
	void Draw(Camera * cam)override;
	void AddToCamera(Camera * camera){}
private:
	Shader m_equirectangularToCubemapShader;
	Shader m_irradianceShader;
	Shader m_prefilterShader;
	Shader m_brdfShader;
	Shader m_backgroundShader;


	// pbr: setup framebuffer
	// ----------------------
	unsigned int captureFBO;
	unsigned int captureRBO;



	unsigned int hdrTexture;

	unsigned int envCubemap;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;


	unsigned int quadVAO = 0;
	unsigned int quadVBO;

public:
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;
	unsigned int irradianceMap;
};

struct SkyBoxManager : public Singleton<SkyBoxManager>
{

	void AddSkyBox(HDRSkyBox* skybox) { m_skybox = skybox; }
	HDRSkyBox * GetSkyBox() { return m_skybox; }
private:
	HDRSkyBox * m_skybox;
};
AUTO_END

