#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Shader.h"

AUTO_BEGIN

class HDR : public Component
{
public:
	explicit HDR(Ambient* ambient);
	~HDR();
	void Start()override;
	void Draw()override;
private:
	Shader m_shader;
	Shader m_hdrShader;

	unsigned int hdrFBO;
	unsigned int woodTexture;
	unsigned int rboDepth;
	unsigned int colorBuffer;

	bool hdr = true;
	bool hdrKeyPressed = false;
	float exposure = 1.0f;

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	unsigned int quadVAO = 0;
	unsigned int quadVBO;
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
};

AUTO_END

