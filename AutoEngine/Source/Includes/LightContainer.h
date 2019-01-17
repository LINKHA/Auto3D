#pragma once
#include "Object.h"

; namespace Auto3D {

class Light;
/**
* @brief : Store all lighting for the current scene
*/
class LightContainer : public Object
{
	REGISTER_OBJECT_CLASS(LightContainer, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit LightContainer(SharedPtr<Ambient> ambient);
	/** 
	* @brief : Add light 
	*/
	void AddLight(SharedPtr<Light> source);
	/**
	* @brief : Remove ligth
	*/
	void RemoveLight(SharedPtr<Light> source);
	/**
	* @brief : Get light container size
	*/
	int Size();
	/**
	* @brief : Get all lights
	* @return : VECTOR(Light*)
	*/
	VECTOR<SharedPtr<Light> > GetAllLights() { return _lights; }
	/**
	* @brief : Get last main light
	*/
	const SharedPtr<Light> GetLastMainLight() { return _lastMainLight; }
	/**
	* @brief : Set is or not render
	*/
	void IsRender(bool b) { _isRenderOrCull = b; }
	/**
	* @brief : Set current light
	*/
	void SetCurrentLight(SharedPtr<Light> light) { _currentLight = light; }
	/**
	* @brief : Get current light
	*/
	SharedPtr<Light> GetCurrentLight() { return _currentLight; }
private:
	/// Is render or cull
	bool _isRenderOrCull;
	/// Current light
	SharedPtr<Light> _currentLight;
	/// Last main light
	SharedPtr<Light> _lastMainLight;
	/// All light
	VECTOR<SharedPtr<Light> > _lights;

};


}