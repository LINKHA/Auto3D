#pragma once
#include "GameManager.h"
#include "Camera.h"
namespace Auto3D {
class Ambient;
class Renderer : public LevelGameManager
{
	REGISTER_DERIVED_CLASS(Renderer, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(Renderer);
	using CameraContainer = _LIST(Camera*);
	///Render in this order
	using ShadowMapContainer = _LIST(RenderComponent*);
	using OpaqueContainer = _LIST(RenderComponent*);
	using CustomContainer = _LIST(RenderComponent*);	//Skybox and other custon component
	using TranslucentContainer = _LIST(RenderComponent*);
	using TranslucentDepth = AUTO_MAP(float, RenderComponent*); //Auxiliary vessel with distance
public:
	explicit Renderer(Ambient* ambient);
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
	
	
private:
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