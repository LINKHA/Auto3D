#pragma once
#include "Texture3D.h"
#include "Singleton.h"
namespace Auto3D {

class HDRSkyBox : public Texture3D
{
public:
	explicit HDRSkyBox(Ambient* ambient);
	~HDRSkyBox();
	void Start()override;
	void Draw()override;
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

class SkyBoxManager : public Singleton<SkyBoxManager>
{
public:
	SkyBoxManager()
		:_enable(false)
	{}
	~SkyBoxManager() = default;

	void AddSkyBox(HDRSkyBox* skybox) { _enable = true; m_skybox = skybox; }
	HDRSkyBox* GetSkyBox() { return m_skybox; }
	bool GetEnable() { return _enable; }
private:
	HDRSkyBox* m_skybox;
	bool _enable;
};
}

