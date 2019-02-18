#include "Matrix3x3.h"

namespace Auto3D {

const Matrix3x3 Matrix3x3::ZERO(
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f);

const Matrix3x3 Matrix3x3::IDENTITY;

Matrix3x3 Matrix3x3::Inverse() const
{
	float det = _m00 * _m11 * _m22 +
		_m10 * _m21 * _m02 +
		_m20 * _m01 * _m12 -
		_m20 * _m11 * _m02 -
		_m10 * _m01 * _m22 -
		_m00 * _m21 * _m12;

	float invDet = 1.0f / det;

	return Matrix3x3(
		(_m11 * _m22 - _m21 * _m12) * invDet,
		-(_m01 * _m22 - _m21 * _m02) * invDet,
		(_m01 * _m12 - _m11 * _m02) * invDet,
		-(_m10 * _m22 - _m20 * _m12) * invDet,
		(_m00 * _m22 - _m20 * _m02) * invDet,
		-(_m00 * _m12 - _m10 * _m02) * invDet,
		(_m10 * _m21 - _m20 * _m11) * invDet,
		-(_m00 * _m21 - _m20 * _m01) * invDet,
		(_m00 * _m11 - _m10 * _m01) * invDet
	);
}

STRING Matrix3x3::ToString() const
{
	char tempBuffer[KhSTL::MATRIX_CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m10, _m11, _m12, _m20, _m21, _m22);
	return STRING(tempBuffer);
}

}
