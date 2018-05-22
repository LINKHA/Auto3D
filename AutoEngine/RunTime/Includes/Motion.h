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


class Motion : public Component
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Motion, Component);
	DECLARE_OBJECT_SERIALIZE(Motion);
public:
	Motion();
	void Enable(bool enable);
	void AddToManager();
	void RemoveFromManager();

	virtual void Awake() {}
	virtual void Start() = 0;
	virtual void Update() = 0;
	virtual void FixUpdate() {}
	virtual void Finish() {}
private:
	bool m_enable;
};

AUTO_END
#endif // !MOTION_H_