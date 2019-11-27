#include "Container/String.h"
#include "Matrix2x2.h"

namespace Auto3D {


const Matrix2x2F Matrix2x2F::ZERO(
	0.0f, 0.0f,
	0.0f, 0.0f);
const Matrix2x2F Matrix2x2F::IDENTITY;

const Matrix2x2I Matrix2x2I::ZERO(
	0, 0,
	0, 0);
const Matrix2x2I Matrix2x2I::IDENTITY;

const Matrix2x2C Matrix2x2C::ZERO(
	0, 0,
	0, 0);
const Matrix2x2C Matrix2x2C::IDENTITY;

const Matrix2x2D Matrix2x2D::ZERO(
	0.0, 0.0,
	0.0, 0.0);
const Matrix2x2D Matrix2x2D::IDENTITY;

}