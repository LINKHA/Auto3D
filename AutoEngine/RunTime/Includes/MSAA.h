#ifndef MSAA_H_
#define MSAA_H_
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
	int m_SamplingPointCount;
private:
	unsigned int ms_quadVAO, ms_quadVBO;
	unsigned int ms_framebuffer;
	unsigned int ms_textureColorBufferMultiSampled;
	unsigned int ms_rbo;
	unsigned int ms_screenTexture;
	unsigned int ms_intermediateFBO;
};
AUTO_END
#endif // !MSAA_H_