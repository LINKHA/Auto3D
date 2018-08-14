#pragma once
#include "ComponentSetting.h"
#include "Shader.h"
namespace Auto3D {
///Temp !!!
enum PostProcessingMode
{
	POST_DEFAULT,
	POST_BULR,
	POST_EDGE_DETECTION,
	POST_GRAYSCALE,
	POST_INVERSION,
	POST_SHARPEN,
};
class OffScreen : public ComponentSetting
{
	REGISTER_DERIVED_CLASS(OffScreen, ComponentSetting);
	DECLARE_OBJECT_SERIALIZE(OffScreen);
public:
	explicit OffScreen(Ambient* ambient);
	/**
	* @brief : Set frame buffer and quad
	*/
	void RenderReady();
	/**
	* @brief : Render before work
	*/
	void RenderStart();
	/**
	* @brief : Render after work
	*/
	void RenderEnd();
	/**
	* @brief : Enable MSAA,and set point numClamp sampling point count(1~8)
	*/
	void AllowMSAA(bool enable, int pointNum = 4);
	/**
	* @brief : Enable Late Effect
	*/
	void AllowLateEffect(bool enable) { _isAllowLateEffect = enable; }
	/**
	* @brief : Enable HDR
	*/
	void AllowHDR(bool enable) { _isAllowHDR = enable; }
	/**
	* @brief : Get MSAA  enable
	*/
	bool GetAllowMSAA() { return _isAllowMsaa; }
	/**
	* @brief : Get late effect enable
	*/
	bool GetAllowLateEffect() { return _isAllowLateEffect; }
	/**
	* @brief : Get hdr enable
	*/
	bool GetAllowHDR() { return _isAllowHDR; }
	/**
	* @brief : Set post processing mode
	*/
	void SetEffect(PostProcessingMode mode);
	/**
	* @brief : SetEffect user-defined shader in effect
	*/
	void SetEffect(const Shader& shader);
private:
	//Temp !!! Hdr cannot be used at the same time as others
#pragma warning
	void bindHdr();
	void bindMsaaAndPostpro();
private:
	Shader shader;
	Shader shaderBlur;
	Shader shaderEdgeDetection;
	Shader shaderGrayscale;
	Shader shaderInversion;
	Shader shaderSharpen;


private:
	bool _isAllowMsaa;
	bool _isAllowLateEffect;
	bool _isAllowHDR;

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