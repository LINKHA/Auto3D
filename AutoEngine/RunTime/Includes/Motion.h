#ifndef MOTION_H_
#define MOTION_H_
#include "GameManager.h"
#include "GameObject.h"

AUTO_BEGIN


class MotionManager : public GlobalGameManager ,public Singleton<MotionManager>
{
	REGISTER_DERIVED_CLASS(MotionManager, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(MotionManager);
	MotionManager();
};

class Component;
class Motion : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Motion, Component);
	DECLARE_OBJECT_SERIALIZE(Motion);
public:
	Motion();
	void Enable(bool enable) { m_enable = enable; }
	bool GetEnable() { return m_enable; }

	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void FixUpdate() {}
	virtual void Finish() {}
private:
	bool m_enable;
};

AUTO_END
#endif // !MOTION_H_