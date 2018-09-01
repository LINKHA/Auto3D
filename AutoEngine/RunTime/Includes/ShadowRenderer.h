#pragma once
#include "Object.h"
#include "Shader.h"

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
	using Ligths = _VECTOR(Light*);
	using RenderComponents = _LIST(RenderComponent*);
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

	Shader& GetDepthMapShader() { return _shadowMapDepthShader; }
	Shader& GetPointDepthMapShader() { return _shadowMapPointDepth; }
private:
	Ligths _lights;
	RenderComponents _shadowComponents;
	Shader _shadowMapDepthShader;
	Shader _shadowMapPointDepth;
	unsigned int _woodTexture;
};

}