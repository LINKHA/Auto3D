#ifndef SHADOW_H_
#define SHADOW_H_
#include "Auto.h"
#include "Shader.h"
#include "GameObject.h"
AUTO_BEGIN
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
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	glm::vec3 lightPos;
	unsigned int depthMapFBO;
	unsigned int depthMap;
};
AUTO_END
#endif //!SHADOW_H_

