#pragma once
#include "GameObject.h"
#include "Shader.h"
AUTO_BEGIN
class HDREquirectangular : public Component
{
public:
	HDREquirectangular();
	~HDREquirectangular();
	void Start()override;
	void Draw(Camera* camera = nullptr)override;
private:
	Shader pbrShader;
	Shader equirectangularToCubemapShader;
	Shader irradianceShader;
	Shader prefilterShader;
	Shader brdfShader;
	Shader backgroundShader;
	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	// pbr: setup framebuffer
	// ----------------------
	unsigned int captureFBO;
	unsigned int captureRBO;

	
	
	unsigned int hdrTexture;

	unsigned int envCubemap;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int sphereVAO = 0;
	unsigned int indexCount;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	void renderSphere();
	void renderCube();
	void renderQuad();

	unsigned int prefilterMap;
	unsigned int brdfLUTTexture;
	unsigned int irradianceMap;
};

AUTO_END