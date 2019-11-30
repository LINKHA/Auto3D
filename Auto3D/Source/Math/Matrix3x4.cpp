#include "Container/String.h"
#include "Matrix3x4.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const TMatrix3x4F TMatrix3x4F::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);
const TMatrix3x4F TMatrix3x4F::IDENTITY;

const TMatrix3x4I TMatrix3x4I::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const TMatrix3x4I TMatrix3x4I::IDENTITY;

const TMatrix3x4C TMatrix3x4C::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const TMatrix3x4C TMatrix3x4C::IDENTITY;

const TMatrix3x4D TMatrix3x4D::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
const TMatrix3x4D TMatrix3x4D::IDENTITY;

}
