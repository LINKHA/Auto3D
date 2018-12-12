#pragma once
#include "Object.h"
namespace Auto3D {

class Engine : public Object
{
	REGISTER_OBJECT_CLASS(Engine, Object)
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
	bool _isExiting{};
};

}