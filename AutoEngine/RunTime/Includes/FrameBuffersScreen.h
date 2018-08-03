#pragma once
#include "Auto.h"
#include "Math/Vector2.h"
#include "Shader.h"
#include "ManagerTool.h"

namespace Auto3D {
enum BuffersMode
{
	kDefault,
	kBlur,
	kEdge_detection,
	kGrayscale,
	kInversion,
	kSharpen,
};
class FrameBuffersScreen : public ManagerTool
{
	REGISTER_DERIVED_CLASS(FrameBuffersScreen, ManagerTool);
	DECLARE_OBJECT_SERIALIZE(FrameBuffersScreen);
public:
	explicit FrameBuffersScreen(Ambient* ambient);

	void SetEffect(BuffersMode mode);
	void SetEffect(const Shader& shader);
	void RenderStart();
	void RenderEnd();

	void Enable(bool enable) { _enable = enable; }
	bool GetEnable() { return _enable; }
private:
	bool _enable;
	Shader _shader;
	Shader shader;
	Shader shaderBlur;
	Shader shaderEdgeDetection;
	Shader shaderGrayscale;
	Shader shaderInversion;
	Shader shaderSharpen;
private:
	unsigned int _framebuffer;
	unsigned int _quadVAO, _quadVBO;
	unsigned int _textureColorbuffer;
	unsigned int _rbo;
private:

};
}

