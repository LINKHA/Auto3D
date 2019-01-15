#pragma once
#include "Component.h"
#include "AutoOGL.h"

namespace Auto3D {

class RenderComponent : public Component
{
	REGISTER_OBJECT_ABSTRACT_CLASS(RenderComponent, Component)
public:
	explicit RenderComponent(SharedPtr<Ambient> ambient);
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
	void RegisterShadow(SharedPtr<RenderComponent> com);
	/**
	* @brief : Register opaque to renderer
	*/
	void RegisterOpaque(SharedPtr<RenderComponent> com);
	/**
	* @brief : Register custom to renderer
	*/
	void RegisterCustom(SharedPtr<RenderComponent> com);
	/**
	* @brief : Register translucent to renderer
	*/
	void RegisterTranslucent(SharedPtr<RenderComponent> com);
	/**
	* @brief : Unload shadow to renderer
	*/
	void UnloadShadow(SharedPtr<RenderComponent> com);
	/**
	* @brief : Unload opaque to renderer
	*/
	void UnloadOpaque(SharedPtr<RenderComponent> com);
	/**
	* @brief : Unload custom to renderer
	*/
	void UnloadCustom(SharedPtr<RenderComponent> com);
	/**
	* @brief : Unload translucent to renderer
	*/
	void UnloadTranslucent(SharedPtr<RenderComponent> com);

};

}
