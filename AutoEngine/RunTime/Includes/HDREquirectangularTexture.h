#pragma once
#include "GameObject.h"
#include "Shader.h"
AUTO_BEGIN
class HDREquirectangularTexture : public Component
{
public:
	HDREquirectangularTexture();
	~HDREquirectangularTexture();
	void Start()override;
	void Draw(Camera* camera = nullptr)override;
private:
	Shader pbrShader;
	Shader equirectangularToCubemapShader;
	Shader irradianceShader;
	Shader prefilterShader;
	Shader brdfShader;
	Shader backgroundShader;

	unsigned int envCubemap;
	unsigned int irradianceMap;
	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderSphere();
	void renderCube();
	void renderQuad();

	unsigned int ironAlbedoMap;
	unsigned int ironNormalMap;
	unsigned int ironMetallicMap;
	unsigned int ironRoughnessMap;
	unsigned int ironAOMap;

	// gold
	unsigned int goldAlbedoMap;
	unsigned int goldNormalMap;
	unsigned int goldMetallicMap;
	unsigned int goldRoughnessMap;
	unsigned int goldAOMap;

	// grass
	unsigned int grassAlbedoMap;
	unsigned int grassNormalMap;
	unsigned int grassMetallicMap;
	unsigned int grassRoughnessMap;
	unsigned int grassAOMap;

	// plastic
	unsigned int plasticAlbedoMap;
	unsigned int plasticNormalMap;
	unsigned int plasticMetallicMap;
	unsigned int plasticRoughnessMap;
	unsigned int plasticAOMap;

	// wall
	unsigned int wallAlbedoMap;
	unsigned int wallNormalMap;
	unsigned int wallMetallicMap;
	unsigned int wallRoughnessMap;
	unsigned int wallAOMap;
};

AUTO_END