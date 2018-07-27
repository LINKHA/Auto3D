#pragma once
#include "Object.h"
AUTO_BEGIN
#define APP_ERROR -1
#define APP_NORMAL 0
#define APP_WARNNING 1
class Engine : public Object
{
	REGISTER_DERIVED_CLASS(Engine, Object);
	DECLARE_OBJECT_SERIALIZE(Engine);
public:
	explicit Engine(Ambient* ambient);
	void Init();
	void RunFrame();
	void Exit();
	bool IsExiting()const { return _isExiting; }
	
	void Render();
private:
	bool _isExiting;
};

AUTO_END