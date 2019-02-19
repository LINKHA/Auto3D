#pragma once
#include "GameManager.h"
#include "AutoOAL.h"

namespace Auto3D {

/** 
* Audio sub system ,Process all sound sources and calculate sound
*/
class Audio : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Audio, GlobalGameManager)
public:
	/**
	* @brief : Construct
	*/
	explicit Audio(SharedPtr<Ambient> ambient);

	/** 
	* @brief : OpenAL device 
	*/
	//ALCdevice* device;
	/**
	* @brief : OpenAL context
	*/
	//ALCcontext* context;

};


}