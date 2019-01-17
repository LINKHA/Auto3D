#pragma once
#include "GameManager.h"
#include "AutoOAL.h"

namespace Auto3D {


class Audio : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Audio, GlobalGameManager)
public:
	/**
	* @brief : Construct
	*/
	explicit Audio(SharedPtr<Ambient> ambient);
	/**
	* @brief : Init audio register audio setting
	*/
	void Init();
	/** 
	* @brief : OpenAL device 
	*/
	ALCdevice* device;
	/**
	* @brief : OpenAL context
	*/
	ALCcontext* context;
};


}