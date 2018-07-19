#pragma once
#include "Auto.h"
#include "Singleton.h"
#include "Shader.h"
AUTO_BEGIN
class MSAA : public Singleton<MSAA>
{
public:
	MSAA();
	~MSAA();

	void Start(int samplingPointCount);
	void UpdateStart();
	void UpdateEnd();
private:
	Shader m_Shader;
	int m_samplingPointCount;
private:
	unsigned int m_quadVAO, m_quadVBO;
	unsigned int m_framebuffer;
	unsigned int m_textureColorBufferMultiSampled;
	unsigned int m_rbo;
	unsigned int m_screenTexture;
	unsigned int m_intermediateFBO;
	
};
AUTO_END
