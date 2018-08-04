#pragma once
#include "Object.h"
namespace Auto3D {
#define APP_ERROR -1
#define APP_NORMAL 0
#define APP_WARNNING 1
class Engine : public Object
{
	REGISTER_DERIVED_CLASS(Engine, Object);
	DECLARE_OBJECT_SERIALIZE(Engine);
public:
	explicit Engine(Ambient* ambient);
	/**
	* @brief : Init engine, Register some SubSystem
	*/
	void Init();
	/**
	* @brief : Run a frame
	*/
	void RunFrame();
	/**
	* @brief : Engine normal exit
	*/
	void Exit();
	/**
	* @brief : Engine exit flag
	*/
	bool IsExiting()const { return _isExiting; }
private:
	/**
	* @brief : Render geometry
	*/
	void render();
	/**
	* @brief : Sub system update data
	*/
	void update();
	/**
	* @brief : Frame finish
	*/
	void frameFinish();
private:
	bool _isExiting;
};

}