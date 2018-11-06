#pragma once
#include "GameManager.h"

namespace Auto3D {


class Audio : public GlobalGameManager
{
	REGISTER_DERIVED_CLASS(Audio, GlobalGameManager);
	DECLARE_OBJECT_SERIALIZE(Audio);
public:
	explicit Audio(Ambient* ambient);
	/**
	* @brief : Init audio register audio setting
	*/
	void Init();

};


}