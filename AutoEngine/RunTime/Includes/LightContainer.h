#pragma once
#include "Object.h"

; namespace Auto3D {

class Light;
/**
* @brief : Store all lighting for the current scene
*/
class LightContainer : public Object
{
	REGISTER_DERIVED_CLASS(LightContainer, Object);
	DECLARE_OBJECT_SERIALIZE(LightContainer);
	using Lights = VECTOR(Light*);
public:
	explicit LightContainer(Ambient* ambient);
	void AddLight(Light* source);
	void RemoveLight(Light* source);
	/**
	* @brief : Get light container size
	*/
	int Size();
	/**
	* @brief : Get all lights
	* @return : VECTOR(Light*)
	*/
	Lights GetAllLights() { return _lights; }
	/**
	* @brief : Get last main light
	*/
	const Light* GetLastMainLight() { return _lastMainLight; }
	/**
	* @brief : Set is or not render
	*/
	void IsRender(bool b) { _isRenderOrCull = b; }
	/**
	* @brief : Set current light
	*/
	void SetCurrentLight(Light* light) { _currentLight = light; }
	/**
	* @brief : Get current light
	*/
	Light* GetCurrentLight() { return _currentLight; }
private:
	bool _isRenderOrCull;
	Light* _currentLight;
	Lights _lights;
	Light* _lastMainLight;

};


}