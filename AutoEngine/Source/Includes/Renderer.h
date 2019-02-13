#pragma once
#include "GameManager.h"
#include "Camera.h"
#include "LightContainer.h"
#include "ShadowRenderer.h"
#include "GraphicsDef.h"

namespace Auto3D {
class tLight;
class RenderPath;
class Geometry;
class Texture2D;
class TextureCube;
class View;
/**
* @brief : Render graphices create to geometry
*/
class Renderer : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Renderer, GlobalGameManager)

	using CameraContainer = LIST<SharedPtr<Camera> >;
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Render in this order
	using ShadowMapContainer = LIST<SharedPtr<RenderComponent> >;
	using OpaqueContainer = LIST<SharedPtr<RenderComponent> >;
	using CustomContainer = LIST<SharedPtr<RenderComponent> >; //Skybox and other custon component
	using TranslucentContainer = LIST<SharedPtr<RenderComponent> >;
	/////////////////////////////////////////////////////////////////////////////////////////////
	///Auxiliary vessel with distance
	using TranslucentDepth = PAIR_MAP<float, SharedPtr<RenderComponent> >;



	using ShadowMapFilter = void(Object::*)(SharedPtr<View> view, SharedPtr<Texture2D> shadowMap, float blurScale);
	
	friend class ShadowRenderer;
	friend class LightContainer;
public:
	explicit Renderer(SharedPtr<Ambient> ambient);
	/// Set default renderpath.
	void SetDefaultRenderPath(RenderPath* renderPath);
	/// Return texture quality level.
	MaterialQuality GetTextureQuality() const { return _textureQuality; }
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
	void AddCamera(SharedPtr<Camera> camera);
	/**
	* @brief : Dynamically remove a camera (valid when traversing the camera)
	*/
	void RemoveCamera(SharedPtr<Camera> camera);
	/**
	* @brief : Dynamically add a shadow map (valid when traversing the camera)
	*/
	void AddShadowMap(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically remove a shadow map (valid when traversing the camera)
	*/
	void RemoveShadowMap(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically add a opaque geometry (valid when traversing the camera)
	*/
	void AddOpaqueGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically remove a opaque geometry (valid when traversing the camera)
	*/
	void RemoveOpaqueGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically add a custom geometry (valid when traversing the camera)
	*/
	void AddCustomGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically remove a custom geometry (valid when traversing the camera)
	*/
	void RemoveCustomGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically add a translucent geometry (valid when traversing the camera)
	*/
	void AddTranslucentGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Dynamically remove a translucent geometry (valid when traversing the camera)
	*/
	void RemoveTranslucentGeometry(SharedPtr<RenderComponent> component);
	/**
	* @brief : Get Current camera quote
	*/
	Camera& GetCurrentCamera() { return *_currentCamera; }
	/**
	* @brief : Get Current camera point
	*/
	SharedPtr<Camera> GetCurrentCameraPtr() { return _currentCamera; }
	/**
	* @brief : Get Current all camera quote
	*/
	CameraContainer GetAllCamera() { return _cameras; }
	/**
	* @brief : Set current camera handle
	*/
	void SetCurrentCamera(SharedPtr<Camera> camera) { _currentCamera = camera; }
	/**
	* @brief : Get light container (friend to LightContainer)
	*/
	SharedPtr<LightContainer> GetLightContainer() { return _lightContainer; }
	/**
	* @brief : Get shadow renderer (friend to ShadowRenderer)
	*/
	SharedPtr<ShadowRenderer> GetShadowRenderer() { return _shadowRenderer; }
	/**
	* @brief : Get All shadow maps (LIST(RenderComponent*))
	*/
	ShadowMapContainer GetAllShadowMaps() { return _shadowsMap; }
	/**
	* @brief : Get All opaques geometrys (LIST(RenderComponent*))
	*/
	OpaqueContainer GetAllOpaques() { return _opaques; }
	/**
	* @brief : Get All custom geometrys (LIST(RenderComponent*))
	*/
	CustomContainer GetAllCustoms() { return _customs; }
	/**
	* @brief : Get All translucent geometrys (LIST(RenderComponent*))
	*/
	TranslucentContainer GetAllTranslucents() { return _translucents; }
	/**
	* @brief : Set shadow quality mode.
	*/
	void SetShadowQuality(ShadowQuality quality);

	/// Set post processing filter to the shadow map
	void SetShadowMapFilter(Object* instance, ShadowMapFilter functionPtr);
	/// Blur the shadow map.
	void BlurShadowMap(SharedPtr<View> view, SharedPtr<Texture2D> shadowMap, float blurScale);


	/// Remove all shadow maps. Called when global shadow map resolution or format is changed.
	void ResetShadowMaps();

private:
	/**
	* @brief : Create light volume geometries
	*/
	void createGeometries();
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
	/// Graphics sub system
	WeakPtr<Graphics> _graphics;

	/// Default renderpath
	SharedPtr<RenderPath> _defaultRenderPath;

	/// Directional light quad geometry
	SharedPtr<Geometry> _dirLightGeometry;
	/// Spot light volume geometry
	SharedPtr<Geometry> _spotLightGeometry;
	/// Point light volume geometry
	SharedPtr<Geometry> _pointLightGeometry;
	/// Texture quality level.
	MaterialQuality _textureQuality{ MaterialQuality::High };
	/// Face selection cube map for shadowed pointlights.
	SharedPtr<TextureCube> _faceSelectCubeMap;

	/// Instance of shadow map filter
	Object* _shadowMapFilterInstance{};
	/// Function pointer of shadow map filter
	ShadowMapFilter _shadowMapFilter{};
	/// Shadow maps by resolution.
	HASH_MAP<int, VECTOR<SharedPtr<Texture2D> > > _shadowMaps;
	/// Shadow map dummy color buffers by resolution.
	HASH_MAP<int, SharedPtr<Texture2D> > _colorShadowMaps;
	/// Shadow map allocations by resolution.
	//HASH_MAP<int, VECTOR<tLight*> > _shadowMapAllocations;

	/// Draw shadows flag
	bool _drawShadows{ true };
	/// Shadow quality
	ShadowQuality _shadowQuality{ ShadowQuality::Pcf16bit };
	/// Shaders need reloading flag
	bool _shadersDirty{ true };


	SharedPtr<ShadowRenderer> _shadowRenderer;
	SharedPtr<LightContainer> _lightContainer;
	///is rendering or culling
	bool _insideRenderOrCull{};
	///camera container
	SharedPtr<Camera> _currentCamera;
	CameraContainer _cameras;
	CameraContainer _camerasToAdd;
	CameraContainer _camerasToRemove;
	///opaques shadow
	SharedPtr<RenderComponent> _currentShadow;
	ShadowMapContainer _shadowsMap;
	ShadowMapContainer _shadowsMapToAdd;
	ShadowMapContainer _shadowsMapToRemove;
	///opaques container
	SharedPtr<RenderComponent> _currentOpaques;
	OpaqueContainer _opaques;
	OpaqueContainer _opaquesToAdd;
	OpaqueContainer _opaquesToRemove;
	///custom container
	SharedPtr<RenderComponent> _currentCustom;
	CustomContainer _customs;
	CustomContainer _customsToAdd;
	CustomContainer _customsToRemove;
	///translucent container
	SharedPtr<RenderComponent> _currentTranslucent;
	TranslucentContainer _translucents;
	TranslucentContainer _translucentsToAdd;
	TranslucentContainer _translucentsRemove;
	TranslucentDepth _translucentsSorted;
};

}