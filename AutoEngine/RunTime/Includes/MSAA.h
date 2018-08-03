#pragma once
#include "Auto.h"
#include "ManagerTool.h"
#include "Shader.h"
namespace Auto3D {
/**
* @brief : Multisampled anti-aliasing, sampling point clamp (1~8)
*/
class MSAA : public ManagerTool
{
	REGISTER_DERIVED_CLASS(MSAA, ManagerTool);
	DECLARE_OBJECT_SERIALIZE(MSAA);
public:
	/**
	* @brief : Set sampling point count
	*/
	explicit MSAA(Ambient* ambient, int pointNum = 4);
	/**
	* @brief : Render before work
	*/
	void RenderStart();
	/**
	* @brief : Render after work
	*/
	void RenderEnd();
private:
	Shader _shader;
	int _samplingPointCount;
	unsigned int _quadVAO, _quadVBO;
	unsigned int _framebuffer;
	unsigned int _textureColorBufferMultiSampled;
	unsigned int _rbo;
	unsigned int _screenTexture;
	unsigned int _intermediateFBO;
};
}
