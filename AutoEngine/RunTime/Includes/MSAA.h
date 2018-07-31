#pragma once
#include "Auto.h"
#include "ManagerTool.h"
#include "Shader.h"
namespace Auto3D {
class MSAA : public ManagerTool
{
public:
	explicit MSAA(Ambient* ambient);
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
}
