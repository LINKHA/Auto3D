#ifndef WORK_SPACE_H_
#define WORK_SPACE_H_
#include "MotionSpace.h"
USING_AUTO
class WorkSpace : public MotionSpace
{
public:
	WorkSpace();
	~WorkSpace();
	void Start()override;
	void Update()override;
	int Launch();
};
#endif //!WORK_SPACE_H_

