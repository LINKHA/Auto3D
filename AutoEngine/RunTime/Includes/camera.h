#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Auto.h"
#include "Motion.h"
AUTO_BEGIN

enum Camera_Movement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

class Camera : public Motion
{
	REGISTER_DERIVED_ABSTRACT_CLASS(Camera, Component);
	DECLARE_OBJECT_SERIALIZE(Camera);
	Camera();
	virtual void Update() {}
	virtual void Start() {}
};

AUTO_END
#endif