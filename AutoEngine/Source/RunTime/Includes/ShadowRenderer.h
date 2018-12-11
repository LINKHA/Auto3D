#pragma once
#include "Object.h"
#include "_Shader.h"

namespace Auto3D {

class Light;
class RenderComponent;
/**
* @brief : Dedicated to renderer draw shadow
*/
class ShadowRenderer : public Object
{

	REGISTER_DERIVED_CLASS(ShadowRenderer, Object);
	DECLARE_OBJECT_SERIALIZE(ShadowRenderer);
	using Ligths = VECTOR<Light*>;
	using RenderComponents = LIST<RenderComponent*>;
public:
	/**
	* @brief : Get renderer to _renderer
	*/
	explicit ShadowRenderer(Ambient* ambient);
	/**
	* @brief : Ready to render shadow(Traversal shadow queue)
	*/
	void ReadyRender();
	/**
	* @brief : Distinguish point light from ambient light
	*	render to shadow(Traversal shadow queue)
	*/
	void RenderShadow();

	_Shader& GetDepthMapShader() { return _shadowMapDepthShader; }
	_Shader& GetPointDepthMapShader() { return _shadowMapPointDepth; }
private:
	Ligths _lights;
	RenderComponents _shadowComponents;
	_Shader _shadowMapDepthShader;
	_Shader _shadowMapPointDepth;
	unsigned int _woodTexture;
};

}