#ifndef WORK_SPACE_H_
#define WORK_SPACE_H_
// OpenGL Header Files
#include "OpenGLGather.h"


//Helper Header Files
#include "Application.h"
#include "AScene.h"
#include "MotionSpace.h"
USING_AUTO
USING_API_AUTO

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

