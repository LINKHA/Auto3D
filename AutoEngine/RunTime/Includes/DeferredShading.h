#pragma once
#include "GameObject.h"
#include "Shader.h"
#include "ModelCommand.h"
#include "stl_use.h"

AUTO_BEGIN
class DeferredShading : public Component
{
public:
	DeferredShading();
	~DeferredShading();
	void Start()override;
	void Draw(Camera* camera = nullptr);
private:
	Shader m_shaderGeometryPass;
	Shader m_shaderLightingPass;
	Shader m_shaderLightBox;

	ModelCommand nanosuit;

	_VECTOR(glm::vec3) objectPositions;
	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec;
	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	unsigned int rboDepth;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;

	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void renderCube();
	void renderQuad();
};
AUTO_END
