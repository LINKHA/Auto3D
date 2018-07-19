#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ModelCommand.h"
#include <random>
AUTO_BEGIN
class SSAO : public Component
{
public:
	SSAO();
	~SSAO();

	void Start()override;
	void Draw()override;

private:
	Shader m_shaderGeometryPass;
	Shader m_shaderLightingPass;
	Shader m_shaderSSAO;
	Shader m_shaderSSAOBlur;

	ModelCommand nanosuit;

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedo;
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int rboDepth;

	unsigned int ssaoFBO, ssaoBlurFBO;
	unsigned int ssaoColorBuffer, ssaoColorBufferBlur;
	unsigned int noiseTexture;
	std::vector<glm::vec3> ssaoKernel;

	std::vector<glm::vec3> ssaoNoise;

	glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
	glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void renderCube();
	void renderQuad();
};
AUTO_END
