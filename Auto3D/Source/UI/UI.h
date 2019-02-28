#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

/// Physics sub system 
class AUTO_API UI : public BaseSubsystem
{
	REGISTER_OBJECT_CLASS(UI, BaseSubsystem)
public:
	/// Construct
	UI();

	~UI();

};


}