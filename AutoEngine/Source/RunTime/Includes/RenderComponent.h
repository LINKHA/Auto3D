#pragma once
#include "Component.h"
#include "AutoOGL.h"

namespace Auto3D {

class RenderComponent : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(RenderComponent, Component)
public:
	explicit RenderComponent(Ambient*  ambient);
	/**
	* @brief : Ready draw component
	*/
	virtual void DrawReady() {}
	/**
	* @brief : Draw shaodw call in renderer
	*/
	virtual void DrawShadow() {}
	/**
	* @brief : Draw call in renderer
	*/
	virtual void Draw() {}

protected:
	/**
	* @brief : Register shadow to renderer
	*/
	void RegisterShadow(RenderComponent* com);
	/**
	* @brief : Register opaque to renderer
	*/
	void RegisterOpaque(RenderComponent* com);
	/**
	* @brief : Register custom to renderer
	*/
	void RegisterCustom(RenderComponent* com);
	/**
	* @brief : Register translucent to renderer
	*/
	void RegisterTranslucent(RenderComponent* com);
	/**
	* @brief : Unload shadow to renderer
	*/
	void UnloadShadow(RenderComponent* com);
	/**
	* @brief : Unload opaque to renderer
	*/
	void UnloadOpaque(RenderComponent* com);
	/**
	* @brief : Unload custom to renderer
	*/
	void UnloadCustom(RenderComponent* com);
	/**
	* @brief : Unload translucent to renderer
	*/
	void UnloadTranslucent(RenderComponent* com);

};

}
