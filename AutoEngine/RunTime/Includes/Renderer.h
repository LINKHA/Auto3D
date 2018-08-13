#pragma once
#include "GameManager.h"
#include "Camera.h"

;
namespace Auto3D {
class Ambient;
class Renderer;
class Light;
/**
* @brief : Store all lighting for the current scene
*/
class LightContainer : public Object
{
	REGISTER_DERIVED_CLASS(LightContainer, Object);
	DECLARE_OBJECT_SERIALIZE(LightContainer);
	using Lights = _VECTOR(Light*);
public:
	explicit LightContainer(Ambient* ambient);
	void AddLight(Light* source);
	void RemoveLight(Light* source);
	int Size();
	Lights GetAllLights() { return _lights; }
	const Light* GetLastMainLight() { return _lastMainLight; }
	void IsRender(bool b) { _isRenderOrCull = b; }
	void SetCurrentLight(Light* light) { _currentLight = light; }
	Light* GetCurrentLight() { return _currentLight; }
private:
	bool _isRenderOrCull;
	Light* _currentLight;
	Lights _lights;
	Light* _lastMainLight;

};
/**
* @brief : Dedicated to renderer draw shadow
*/
//Temp
class MeshShadow;

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
	void ReadyRender();
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
/**
* @brief : Render graphices create to geometry
*/
class Renderer : public LevelGameManager
{
	REGISTER_DERIVED_CLASS(Renderer, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(Renderer);
	using CameraContainer = _LIST(Camera*);
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Render in this order
	using ShadowMapContainer = _LIST(RenderComponent*);
	using OpaqueContainer = _LIST(RenderComponent*);
	using CustomContainer = _LIST(RenderComponent*); //Skybox and other custon component
	using TranslucentContainer = _LIST(RenderComponent*);
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Auxiliary vessel with distance
	using TranslucentDepth = AUTO_MAP(float, RenderComponent*); 
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
	LightContainer* GetLightContainer() { return _lightContainer; }
	/**
	* @brief : Get shadow renderer (friend to ShadowRenderer)
	*/
	ShadowRenderer* GetShadowRenderer() { return _shadowRenderer; }
	/**
	* @brief : Get All shadow maps (_LIST(RenderComponent*))
	*/
	ShadowMapContainer& GetAllShadowMaps() { return _shadowsMap; }
	/**
	* @brief : Get All opaques geometrys (_LIST(RenderComponent*))
	*/
	OpaqueContainer& GetAllOpaques() { return _opaques; }
	/**
	* @brief : Get All custom geometrys (_LIST(RenderComponent*))
	*/
	CustomContainer& GetAllCustoms() { return _customs; }
	/**
	* @brief : Get All translucent geometrys (_LIST(RenderComponent*))
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