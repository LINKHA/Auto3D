#pragma once
#include "DyDlobal.h"
namespace Auot3D
{

	GLUSboolean initBackground();

	GLUSvoid reshapeBackground(GLUSfloat projectionMatrix[16]);

	GLUSboolean renderBackground(GLUSfloat viewMatrix[16]);

	GLUSvoid terminateBackground(GLUSvoid);



}