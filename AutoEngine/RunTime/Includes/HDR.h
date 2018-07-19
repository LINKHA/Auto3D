#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "Shader.h"

AUTO_BEGIN

class HDR : public Component
{
public:
	HDR();
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

	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
};

AUTO_END

