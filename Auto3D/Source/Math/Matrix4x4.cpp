#include "Container/String.h"
#include "Matrix4x4.h"
#include "Matrix3x4.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const TMatrix4x4F TMatrix4x4F::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);
const TMatrix4x4F TMatrix4x4F::IDENTITY;

const TMatrix4x4I TMatrix4x4I::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const TMatrix4x4I TMatrix4x4I::IDENTITY;

const TMatrix4x4C TMatrix4x4C::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const TMatrix4x4C TMatrix4x4C::IDENTITY;

const TMatrix4x4D TMatrix4x4D::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
const TMatrix4x4D TMatrix4x4D::IDENTITY;




}
