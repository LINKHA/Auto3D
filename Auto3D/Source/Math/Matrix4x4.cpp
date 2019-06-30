#include "../Container/String.h"
#include "Matrix4x4.h"
#include "Matrix3x4.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Matrix4x4F Matrix4x4F::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);
const Matrix4x4F Matrix4x4F::IDENTITY;

const Matrix4x4I Matrix4x4I::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix4x4I Matrix4x4I::IDENTITY;

const Matrix4x4C Matrix4x4C::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix4x4C Matrix4x4C::IDENTITY;

const Matrix4x4D Matrix4x4D::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
const Matrix4x4D Matrix4x4D::IDENTITY;




}
