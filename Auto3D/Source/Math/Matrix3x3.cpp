#include "../Container/String.h"
#include "Matrix3x3.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{
const Matrix3x3F Matrix3x3F::ZERO(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);
const Matrix3x3F Matrix3x3F::IDENTITY;

const Matrix3x3I Matrix3x3I::ZERO(
	0, 0, 0,
	0, 0, 0,
	0, 0, 0);
const Matrix3x3I Matrix3x3I::IDENTITY;

const Matrix3x3C Matrix3x3C::ZERO(
	0, 0, 0,
	0, 0, 0,
	0, 0, 0);
const Matrix3x3C Matrix3x3C::IDENTITY;

const Matrix3x3D Matrix3x3D::ZERO(
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0);
const Matrix3x3D Matrix3x3D::IDENTITY;

}
