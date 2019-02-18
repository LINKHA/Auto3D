#include "Matrix2x2.h"

namespace Auto3D{


const Matrix2x2 Matrix2x2::ZERO(
	0.0f, 0.0f,
	0.0f, 0.0f);

const Matrix2x2 Matrix2x2::IDENTITY;

Matrix2x2 Matrix2x2::Inverse() const
{
	float det = _m00 * _m11 -
		_m01 * _m10;

	float invDet = 1.0f / det;

	return Matrix2x2(
		_m11, -_m01,
		-_m10, _m00
	) * invDet;
}

STRING Matrix2x2::ToString() const
{
	char tempBuffer[KhSTL::MATRIX_CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _m00, _m01, _m10, _m11);
	return STRING(tempBuffer);
}


}