#ifndef PARALLAX_MAPPING_SPACE_H_
#define PARALLAX_MAPPING_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class ParallaxMappingSpace : public MotionSpace
{
public:
	ParallaxMappingSpace();
	~ParallaxMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!PARALLAX_MAPPING_SPACE_H_

