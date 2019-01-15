#pragma once
#include "GameManager.h"
#include "AutoOAL.h"

namespace Auto3D {


class Audio : public GlobalGameManager
{
	REGISTER_OBJECT_CLASS(Audio, GlobalGameManager)
public:
	explicit Audio(SharedPtr<Ambient> ambient);
	/**
	* @brief : Init audio register audio setting
	*/
	void Init();
	ALCdevice* device;
	ALCcontext* context;
};


}