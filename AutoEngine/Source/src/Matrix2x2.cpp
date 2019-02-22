#include "Matrix2x2.h"

namespace Auto3D{


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

template<typename _Ty> Matrix2x2<_Ty> Matrix2x2<_Ty>::Inverse() const
{
	float det = _m00 * _m11 -
		_m01 * _m10;

	float invDet = 1.0f / det;

	return Matrix2x2<_Ty>(
		_m11, -_m01,
		-_m10, _m00
	) * invDet;
}

template<typename _Ty> STRING Matrix2x2<_Ty>::ToString() const
{
	char tempBuffer[KhSTL::MATRIX_CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _m00, _m01, _m10, _m11);
	return STRING(tempBuffer);
}


}