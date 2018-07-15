#pragma once
#include "GameObject.h"
#include "Shader.h"
AUTO_BEGIN

class Bloom : public Component
{
public:
	Bloom();
	~Bloom();
	void Start()override;
	void Draw(Camera* camera = nullptr);
private:
	Shader m_shader;
	Shader m_shaderLight;
	Shader m_shaderBlur;
	Shader m_shaderBloomFinal;

	unsigned int woodTexture;
	unsigned int containerTexture;

	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int rboDepth;
	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	
	unsigned int pingpongFBO[2];
	unsigned int pingpongColorbuffers[2];

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;

	bool bloom = true;
	bool bloomKeyPressed = false;
	float exposure = 1.0f;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	// -----------------------------------------
	unsigned int quadVAO = 0;
	unsigned int quadVBO;

	void renderCube();
	void renderQuad();


};

AUTO_END
