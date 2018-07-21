#pragma once

#include "GameManager.h"
#include "Light.h"
#include "Singleton.h"
AUTO_BEGIN
class Light;
class LightManager : public LevelGameManager ,public Singleton<LightManager>
{
	REGISTER_DERIVED_CLASS(LightManager, LevelGameManager);
	DECLARE_OBJECT_SERIALIZE(LightManager);
	typedef _VECTOR(Light*) Lights;
public:
	LightManager();
	void AddLight(Light* source);
	void RemoveLight(Light* source);
	int Size();
	Lights& GetAllLights() { return _lights; }
	const Light* GetLastMainLight() { return _lastMainLight; }
	void IsRender(bool b) { _isRenderOrCull = b; }
private:
	bool		_isRenderOrCull;
	Lights		_lights;
	Light*		_lastMainLight;
};
AUTO_END

