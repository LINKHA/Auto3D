#ifndef MULT_LIGHTS_SPACE_H_
#define MULT_LIGHTS_SPACE_H_

#include "MotionSpace.h"
USING_AUTO

class MultLightsSpace : public MotionSpace
{
public:
	MultLightsSpace();
	~MultLightsSpace();
	void Start()override;
	void Update()override;
	int Launch();

	
};
#endif //!MULT_LIGHTS_SPACE_H_

