#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class DebugSapce : public MotionSpace
{
public: 
		using This = DebugSapce; 
		using Super = MotionSpace; 
		DebugSapce(SharedPtr<Ambient> ambient)
			:MotionSpace(ambient) 
		{}
		~DebugSapce()
		{}
public:
	void Init()override;
	void Destruct()override;
};


