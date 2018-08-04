#pragma once
#include "GameObject.h"
#include "Shader.h"
namespace Auto3D {
class ShadowPoint : public Component
{
public:
	explicit ShadowPoint(Ambient* ambient);
	~ShadowPoint();
	void Start()override;
	void Draw()override;
private:
	Shader m_ShadowMap;
	Shader m_ShadowMapDepth;
	glm::vec3 lightPos;
	unsigned int woodTexture;
	unsigned int planeVAO;
	unsigned int loadTexture(const char* path);
	void renderScene(const Shader& shader);
	void renderCube();


};

}
