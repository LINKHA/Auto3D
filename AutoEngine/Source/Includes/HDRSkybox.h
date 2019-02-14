#pragma once
#include "tTexture3D.h"
#include "ShaderVariation.h"

namespace Auto3D {
class tCamera;

class HDRSkyBox : public tTexture3D
{
public:
	explicit HDRSkyBox(SharedPtr<Ambient> ambient);
	~HDRSkyBox();
	void Destory()override;
	void Start()override;
	void Draw()override;
	void AddToCamera(tCamera* camera){}
private:
	SharedPtr<ShaderVariation> m_equirectangularToCubemapShader;
	SharedPtr<ShaderVariation> m_irradianceShader;
	SharedPtr<ShaderVariation> m_prefilterShader;
	SharedPtr<ShaderVariation> m_brdfShader;
	SharedPtr<ShaderVariation> m_backgroundShader;


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

class SkyBoxManager
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

