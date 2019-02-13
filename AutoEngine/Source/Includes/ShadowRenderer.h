#pragma once
#include "Object.h"
#include "ShaderVariation.h"

namespace Auto3D {

class tLight;
class RenderComponent;
/**
* @brief : Dedicated to renderer draw shadow
*/
class ShadowRenderer : public Object
{
	REGISTER_OBJECT_CLASS(ShadowRenderer, Object)
public:
	/**
	* @brief : Get renderer to renderer
	*/
	explicit ShadowRenderer(SharedPtr<Ambient> ambient);
	/**
	* @brief : Ready to render shadow(Traversal shadow queue)
	*/
	void ReadyRender();
	/**
	* @brief : Distinguish point light from ambient light
	*	render to shadow(Traversal shadow queue)
	*/
	void RenderShadow();
	/** 
	* @brief : Get depth map shader 
	*/
	SharedPtr<ShaderVariation> GetDepthMapShader() { return _shadowMapDepthShader; }
	/**
	* @brief : Get point depth map shader
	*/
	SharedPtr<ShaderVariation> GetPointDepthMapShader() { return _shadowMapPointDepth; }
private:
	/// Lights
	VECTOR<SharedPtr<tLight> > _lights;
	/// Shadow component
	LIST<SharedPtr<RenderComponent> > _shadowComponents;
	/// Shadow map depth shader
	SharedPtr<ShaderVariation> _shadowMapDepthShader;
	/// Shadow map point depth
	SharedPtr<ShaderVariation> _shadowMapPointDepth;
	/// Wood texture
	unsigned int _woodTexture;
};

}