#pragma once
#include "GameManager.h"
#include "Camera.h"
#include "LightContainer.h"
#include "ShadowRenderer.h"
#include "AutoSTL.h"

namespace Auto3D {
class Ambient;
class Light;
/**
* @brief : Render graphices create to geometry
*/
class Renderer : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Renderer, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Renderer);
	using CameraContainer = LIST<Camera*>;
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Render in this order
	using ShadowMapContainer = LIST<RenderComponent*>;
	using OpaqueContainer = LIST<RenderComponent*>;
	using CustomContainer = LIST<RenderComponent*>; //Skybox and other custon component
	using TranslucentContainer = LIST<RenderComponent*>;
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Auxiliary vessel with distance
	using TranslucentDepth = PAIR_MAP<float, RenderComponent*>; 
	friend class ShadowRenderer;
	friend class LightContainer;
public:
	explicit Renderer(Ambient* ambient);
	/**
	* @brief : Init renderer register renderer sub setting
	*/
	void Init();
	/**
	* @brief : Ready to renderer create shadow frame buffer..
	*/
	void ReadyToRender();
	/**
	* @brief : Render every camera
	*/
	void Render();
	/**
	* @brief : Dynamically add a camera (valid when traversing the camera)
	*/
	void AddCamera(Camera* c);
	/**
	* @brief : Dynamically remove a camera (valid when traversing the camera)
	*/
	void RemoveCamera(Camera* c);
	/**
	* @brief : Dynamically add a shadow map (valid when traversing the camera)
	*/
	void AddShadowMap(RenderComponent* component);
	/**
	* @brief : Dynamically remove a shadow map (valid when traversing the camera)
	*/
	void RemoveShadowMap(RenderComponent* component);
	/**
	* @brief : Dynamically add a opaque geometry (valid when traversing the camera)
	*/
	void AddOpaqueGeometry(RenderComponent* component);
	/**
	* @brief : Dynamically remove a opaque geometry (valid when traversing the camera)
	*/
	void RemoveOpaqueGeometry(RenderComponent* component);
	/**
	* @brief : Dynamically add a custom geometry (valid when traversing the camera)
	*/
	void AddCustomGeometry(RenderComponent* component);
	/**
	* @brief : Dynamically remove a custom geometry (valid when traversing the camera)
	*/
	void RemoveCustomGeometry(RenderComponent* component);
	/**
	* @brief : Dynamically add a translucent geometry (valid when traversing the camera)
	*/
	void AddTranslucentGeometry(RenderComponent* component);
	/**
	* @brief : Dynamically remove a translucent geometry (valid when traversing the camera)
	*/
	void RemoveTranslucentGeometry(RenderComponent* component);
	/**
	* @brief : Get Current camera quote
	*/
	Camera& GetCurrentCamera() { return *_currentCamera; }
	/**
	* @brief : Get Current camera point
	*/
	Camera* GetCurrentCameraPtr() { return _currentCamera; }
	/**
	* @brief : Get Current all camera quote
	*/
	CameraContainer& GetAllCamera() { return _cameras; }
	/**
	* @brief : Set current camera handle
	*/
	void SetCurrentCamera(Camera* c) { _currentCamera = c; }
	/**
	* @brief : Get light container (friend to LightContainer)
	*/
	LightContainer* GetLightContainer() { return _lightContainer.get(); }
	/**
	* @brief : Get shadow renderer (friend to ShadowRenderer)
	*/
	ShadowRenderer* GetShadowRenderer() { return _shadowRenderer.get(); }
	/**
	* @brief : Get All shadow maps (LIST(RenderComponent*))
	*/
	ShadowMapContainer& GetAllShadowMaps() { return _shadowsMap; }
	/**
	* @brief : Get All opaques geometrys (LIST(RenderComponent*))
	*/
	OpaqueContainer& GetAllOpaques() { return _opaques; }
	/**
	* @brief : Get All custom geometrys (LIST(RenderComponent*))
	*/
	CustomContainer& GetAllCustoms() { return _customs; }
	/**
	* @brief : Get All translucent geometrys (LIST(RenderComponent*))
	*/
	TranslucentContainer& GetAllTranslucents() { return _translucents; }
private:
	/**
	* @brief : Delay add or remove camera
	*/
	void delayedAddRemoveCameras();
	/**
	* @brief : Delay add or remove shadow map
	*/
	void delayedAddRemoveShadowMaps();
	/**
	* @brief : Delay add or remove opaque geometry
	*/
	void delayedAddRemoveOpaques();
	/**
	* @brief : Delay add or remove custom geometry
	*/
	void delayedAddRemoveCustoms();
	/**
	* @brief : Delay add or remove translucent geometry
	*/
	void delayedAddRemoveTranslucents();
	/**
	* @brief : Delay add or remove shadow map
	*/
	void renderShadowMap();
	/**
	* @brief : Render opaques geometry
	*/
	void renderOpaques();
	/**
	* @brief : Render custom geometry
	*/
	void renderCustom();
	/**
	* @brief : Render translucent geometry
	*/
	void renderTranslucent();
	/**
	* @brief : Sort translucent geometry from depth test
	*/
	void translucentGeometrySort();
	/**
	* @brief : Intelligence mout shadow renderer,Intelligent creation object
	*/
	void intelMoutShadwoRenderer();
	/**
	* @brief : Intelligence mout light container,Intelligent creation object
	*/
	void intelMoutLightContainer();
private:
	SharedPtr<ShadowRenderer> _shadowRenderer;
	SharedPtr<LightContainer> _lightContainer;
	///is rendering or culling
	bool _insideRenderOrCull;
	///camera container
	Camera* _currentCamera;
	CameraContainer _cameras;
	CameraContainer _camerasToAdd;
	CameraContainer _camerasToRemove;
	///opaques shadow
	RenderComponent* _currentShadow;
	ShadowMapContainer _shadowsMap;
	ShadowMapContainer _shadowsMapToAdd;
	ShadowMapContainer _shadowsMapToRemove;
	///opaques container
	RenderComponent* _currentOpaques;
	OpaqueContainer _opaques;
	OpaqueContainer _opaquesToAdd;
	OpaqueContainer _opaquesToRemove;
	///custom container
	RenderComponent* _currentCustom;
	CustomContainer _customs;
	CustomContainer _customsToAdd;
	CustomContainer _customsToRemove;
	///translucent container
	RenderComponent* _currentTranslucent;
	TranslucentContainer _translucents;
	TranslucentContainer _translucentsToAdd;
	TranslucentContainer _translucentsRemove;
	TranslucentDepth _translucentsSorted;
};

}