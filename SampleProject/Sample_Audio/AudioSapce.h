#pragma  once
#include "MotionSpace.h"
using namespace Auto3D;
class AudioSapce : public MotionSpace
{
public:
	explicit AudioSapce(Ambient* ambient);
	~AudioSapce();
	void Start()override;
	void Update()override;

};