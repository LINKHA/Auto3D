#ifndef MOTION_SPACE_H_
#define MOTION_SPACE_H_
#include "Auto.h"
#include "ObjectDefines.h"
#include "Singleton.h"
#include "Mesh.h"
AUTO_BEGIN
/**
* @brief Singleton class
*/
class MotionSpace : public Singleton<MotionSpace>
{
public:
	MotionSpace();
	~MotionSpace();
	void SetWindow(GLFWwindow* glfwWindow);
	void Awake();
	void Start(); // use
	void Update(); // use
	void FixUpdate();
	void Finish();
};

AUTO_END

#endif // !MOTION_SPACE_H_
