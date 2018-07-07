#ifndef FRAME_BUFFERS_SCREEN_H_
#define FRAME_BUFFERS_SCREEN_H_
#include "Auto.h"
#include "Math/Vector2.h"
#include "Shader.h"
#include "Singleton.h"
USING_MATH
AUTO_BEGIN
enum BuffersMode
{
	Default,
	Blur,
	Edge_detection,
	Grayscale,
	Inversion,
	Sharpen,
};
class FrameBuffersScreen : public Singleton<FrameBuffersScreen>
{
public:
	FrameBuffersScreen();
	~FrameBuffersScreen();
	void SetEffect(BuffersMode mode);
	void SetEffect(const Shader& shader);
	void Start();
	void DrawStart();
	void DrawEnd();
	void Enable(bool enable) { m_enable = enable; }
	bool GetEnable() { return m_enable; }
private:
	bool m_enable;
	Shader m_shader;
private:
	unsigned int fb_framebuffer;
	unsigned int fb_quadVAO, fb_quadVBO;
	unsigned int fb_textureColorbuffer;
	unsigned int fb_rbo;
private:

};
AUTO_END
#endif // FRAME_BUFFERS_SCREEN_H_
