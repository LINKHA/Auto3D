#include "Container/String.h"
#include "Matrix3x3.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{
const TMatrix3x3F TMatrix3x3F::ZERO(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);
const TMatrix3x3F TMatrix3x3F::IDENTITY;

const TMatrix3x3I TMatrix3x3I::ZERO(
	0, 0, 0,
	0, 0, 0,
	0, 0, 0);
const TMatrix3x3I TMatrix3x3I::IDENTITY;

const TMatrix3x3C TMatrix3x3C::ZERO(
	0, 0, 0,
	0, 0, 0,
	0, 0, 0);
const TMatrix3x3C TMatrix3x3C::IDENTITY;

const TMatrix3x3D TMatrix3x3D::ZERO(
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0,
	0.0, 0.0, 0.0);
const TMatrix3x3D TMatrix3x3D::IDENTITY;

}
