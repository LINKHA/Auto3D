#pragma once
#include "Auto.h"
#include "GLStateCache.h"
#include "Shader.h"
AUTO_BEGIN
class Camera;
class ShadowTest
{
public:
	ShadowTest();
	~ShadowTest();
	void Start();
	void UpdateStart(Camera * camera);
	void UpdateEnd(Camera * camera);
private:
	Shader m_shadowShader;
	GLuint m_depthMapFBO;
	GLuint m_depthMap;
private:
	int shadowWidth;
	int shadowHeight;
};
AUTO_END
