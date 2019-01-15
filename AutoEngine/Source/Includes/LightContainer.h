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
	explicit LightContainer(SharedPtr<Ambient> ambient);
	void AddLight(SharedPtr<Light> source);
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
	bool _isRenderOrCull;
	SharedPtr<Light> _currentLight;
	SharedPtr<Light> _lastMainLight;

	VECTOR<SharedPtr<Light> > _lights;

};


}