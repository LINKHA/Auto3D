#ifndef NORMAL_MAPPING_SPACE_H_
#define NORMAL_MAPPING_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class NormalMappingSpace : public MotionSpace
{
public:
	NormalMappingSpace();
	~NormalMappingSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!NORMAL_MAPPING_SPACE_H_

