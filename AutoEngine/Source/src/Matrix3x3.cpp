#include "Matrix3x3.h"

namespace Auto3D {

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

template<typename _Ty> Matrix3x3<_Ty> Matrix3x3<_Ty>::Inverse() const
{
	_Ty det = _m00 * _m11 * _m22 +
		_m10 * _m21 * _m02 +
		_m20 * _m01 * _m12 -
		_m20 * _m11 * _m02 -
		_m10 * _m01 * _m22 -
		_m00 * _m21 * _m12;

	_Ty invDet = 1.0f / det;

	return Matrix3x3<_Ty>(
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

template<typename _Ty> STRING Matrix3x3<_Ty>::ToString() const
{
	char tempBuffer[KhSTL::MATRIX_CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m10, _m11, _m12, _m20, _m21, _m22);
	return STRING(tempBuffer);
}

}
