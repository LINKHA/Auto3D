#pragma once
#include "DyDlobal.h"
namespace Auto3D
{

	GLUSboolean initBackground();

	GLUSvoid reshapeBackground(GLUSfloat projectionMatrix[16]);

	GLUSboolean renderBackground(GLUSfloat viewMatrix[16]);

}