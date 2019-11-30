#include "Container/String.h"
#include "Matrix2x2.h"

namespace Auto3D {


const TMatrix2x2F TMatrix2x2F::ZERO(
	0.0f, 0.0f,
	0.0f, 0.0f);
const TMatrix2x2F TMatrix2x2F::IDENTITY;

const TMatrix2x2I TMatrix2x2I::ZERO(
	0, 0,
	0, 0);
const TMatrix2x2I TMatrix2x2I::IDENTITY;

const TMatrix2x2C TMatrix2x2C::ZERO(
	0, 0,
	0, 0);
const TMatrix2x2C TMatrix2x2C::IDENTITY;

const TMatrix2x2D TMatrix2x2D::ZERO(
	0.0, 0.0,
	0.0, 0.0);
const TMatrix2x2D TMatrix2x2D::IDENTITY;

}