#pragma once
#include "Object.h"

; namespace Auto3D {

class tLight;
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
	void AddLight(SharedPtr<tLight> source);
	/**
	* @brief : Remove ligth
	*/
	void RemoveLight(SharedPtr<tLight> source);
	/**
	* @brief : Get light container size
	*/
	int Size();
	/**
	* @brief : Get all lights
	* @return : VECTOR(Light*)
	*/
	VECTOR<SharedPtr<tLight> > GetAllLights() { return _lights; }
	/**
	* @brief : Get last main light
	*/
	const SharedPtr<tLight> GetLastMainLight() { return _lastMainLight; }
	/**
	* @brief : Set is or not render
	*/
	void IsRender(bool b) { _isRenderOrCull = b; }
	/**
	* @brief : Set current light
	*/
	void SetCurrentLight(SharedPtr<tLight> light) { _currentLight = light; }
	/**
	* @brief : Get current light
	*/
	SharedPtr<tLight> GetCurrentLight() { return _currentLight; }
private:
	/// Is render or cull
	bool _isRenderOrCull;
	/// Current light
	SharedPtr<tLight> _currentLight;
	/// Last main light
	SharedPtr<tLight> _lastMainLight;
	/// All light
	VECTOR<SharedPtr<tLight> > _lights;

};


}