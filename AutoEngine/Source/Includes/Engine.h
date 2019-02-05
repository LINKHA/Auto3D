#pragma once
#include "Object.h"

namespace Auto3D {

/** 
* Auto Engine manage render update,create subsystem
*/
class Engine : public Object
{
	REGISTER_OBJECT_CLASS(Engine, Object)
public:
	/**
	* @brief : Construct
	*/
	explicit Engine(SharedPtr<Ambient> ambient);
	/**
	* @brief : Init engine, Register some SubSystem
	*/
	void Init();
	/**
	* @brief : Engine normal exit
	*/
	void Exit();
	/** 
	* @brief : Shut down engine 
	*/
	void ShutDownEngine() { _isExiting = true; }
	/**
	* @brief : Engine exit flag
	*/
	bool IsExiting()const { return _isExiting; }
	/**
	* @brief : Render geometry
	*/
	void Render();
	/**
	* @brief : Sub system update data
	*/
	void Update();
	/**
	* @brief : Frame finish
	*/
	void FrameFinish();
private:
	/// Is exiting
	bool _isExiting{};

};

}