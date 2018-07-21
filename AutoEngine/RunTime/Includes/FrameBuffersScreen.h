#pragma once
#include "Auto.h"
#include "Math/Vector2.h"
#include "Shader.h"
#include "Singleton.h"
USING_MATH
AUTO_BEGIN
enum BuffersMode
{
	kDefault,
	kBlur,
	kEdge_detection,
	kGrayscale,
	kInversion,
	kSharpen,
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
	void Enable(bool enable) { _enable = enable; }
	bool GetEnable() { return _enable; }
private:
	bool _enable;
	Shader _shader;
private:
	unsigned int _framebuffer;
	unsigned int _quadVAO, _quadVBO;
	unsigned int _textureColorbuffer;
	unsigned int _rbo;
private:

};
AUTO_END

