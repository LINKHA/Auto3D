#pragma once
#include "Object.h"
#include "ShaderVariation.h"

namespace Auto3D {

class Light;
class RenderComponent;
/**
* @brief : Dedicated to renderer draw shadow
*/
class ShadowRenderer : public Object
{
	REGISTER_OBJECT_CLASS(ShadowRenderer, Object)

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

	ShaderVariation* GetDepthMapShader() { return _shadowMapDepthShader.get(); }
	ShaderVariation* GetPointDepthMapShader() { return _shadowMapPointDepth.get(); }
private:
	VECTOR<Light*> _lights;
	RenderComponents _shadowComponents;
	SharedPtr<ShaderVariation> _shadowMapDepthShader;
	SharedPtr<ShaderVariation> _shadowMapPointDepth;
	unsigned int _woodTexture;
};

}