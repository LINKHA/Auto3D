#pragma once
#include "DyDlobal.h"
namespace Auto3D
{

	typedef struct _WaveParameters
	{
		GLfloat speed;
		GLfloat amplitude;
		GLfloat wavelength;
		GLfloat steepness;
	} WaveParameters;

	typedef struct _WaveDirections
	{
		GLfloat x;
		GLfloat z;
	} WaveDirections;

	GLUSuint initWaterTexture(GLUSfloat waterPlaneLength);

	GLUSvoid reshapeWaterTexture(GLUSint width, GLUSint height);

	GLUSboolean renderWaterTexture(GLUSfloat time);

}