#pragma once
#include "GameObject.h"
#include "Shader.h"
AUTO_BEGIN
class ShadowPoint : public Component
{
public:
	ShadowPoint();
	~ShadowPoint();
	void Start()override;
	void Draw(Camera* camera = nullptr)override;
private:
	Shader m_ShadowMap;
	Shader m_ShadowMapDepth;
	glm::vec3 lightPos;
	unsigned int woodTexture;
	unsigned int planeVAO;
	unsigned int loadTexture(const char *path);
	void renderScene(const Shader &shader);
	void renderCube();


};

AUTO_END
