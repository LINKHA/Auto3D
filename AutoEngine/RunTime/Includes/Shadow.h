#ifndef SHADOW_H_
#define SHADOW_H_
#include "Auto.h"
#include "Shader.h"
#include "GameObject.h"
AUTO_BEGIN
#define SHADOW_DEBUG 1
#if SHADOW_DEBUG

class Shadow : public Component
{
public:
	Shadow(int i);
	virtual ~Shadow();
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
	int k;
	unsigned int cubeVAO = 0;
	unsigned int cubeVBO;
};

#else
class Shadow : public Component
{
public:
	Shadow();
	virtual ~Shadow();
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
#endif
AUTO_END
#endif //!SHADOW_H_

