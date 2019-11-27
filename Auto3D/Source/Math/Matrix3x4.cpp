#include "Container/String.h"
#include "Matrix3x4.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const Matrix3x4F Matrix3x4F::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);
const Matrix3x4F Matrix3x4F::IDENTITY;

const Matrix3x4I Matrix3x4I::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix3x4I Matrix3x4I::IDENTITY;

const Matrix3x4C Matrix3x4C::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix3x4C Matrix3x4C::IDENTITY;

const Matrix3x4D Matrix3x4D::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
const Matrix3x4D Matrix3x4D::IDENTITY;

}
