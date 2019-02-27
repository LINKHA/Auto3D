#pragma once
#include "../Object/GameManager.h"

namespace Auto3D {

/// Physics sub system 
class AUTO_API UI : public Subsystem
{
	REGISTER_OBJECT_CLASS(UI, Subsystem)
public:
	/// Construct
	UI();

	~UI();

};


}