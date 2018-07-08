#ifndef LIGHT_MANAGER_H_
#define LIGHT_MANAGER_H_

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
	Lights& GetAllLights() { return m_Lights; }
	const Light* GetLastMainLight() { return m_LastMainLight; }
	void IsRender(bool b) { m_RenderOrCull = b; }
private:
	bool		m_RenderOrCull;
	Lights		m_Lights;
	Light*	m_LastMainLight;
};
AUTO_END
#endif // !LIGHT_MANAGER_H_
