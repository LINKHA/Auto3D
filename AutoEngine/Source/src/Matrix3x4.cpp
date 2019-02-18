#include "Matrix3x4.h"

namespace Auto3D
{

	const Matrix3x4 Matrix3x4::ZERO(
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f);

	const Matrix3x4 Matrix3x4::IDENTITY;

	void Matrix3x4::Decompose(Vector3& translation, Quaternion& rotation, Vector3& scale) const
	{
		translation.x = _m03;
		translation.y = _m13;
		translation.z = _m23;

		scale.x = sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20);
		scale.y = sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21);
		scale.z = sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22);

		Vector3 invScale(1.0f / scale.x, 1.0f / scale.y, 1.0f / scale.z);
		rotation = Quaternion(ToMatrix3().Scaled(invScale));
	}

	Matrix3x4 Matrix3x4::Inverse() const
	{
		float det = _m00 * _m11 * _m22 +
			_m10 * _m21 * _m02 +
			_m20 * _m01 * _m12 -
			_m20 * _m11 * _m02 -
			_m10 * _m01 * _m22 -
			_m00 * _m21 * _m12;

		float invDet = 1.0f / det;
		Matrix3x4 ret;

		ret._m00 = (_m11 * _m22 - _m21 * _m12) * invDet;
		ret._m01 = -(_m01 * _m22 - _m21 * _m02) * invDet;
		ret._m02 = (_m01 * _m12 - _m11 * _m02) * invDet;
		ret._m03 = -(_m03 * ret._m00 + _m13 * ret._m01 + _m23 * ret._m02);
		ret._m10 = -(_m10 * _m22 - _m20 * _m12) * invDet;
		ret._m11 = (_m00 * _m22 - _m20 * _m02) * invDet;
		ret._m12 = -(_m00 * _m12 - _m10 * _m02) * invDet;
		ret._m13 = -(_m03 * ret._m10 + _m13 * ret._m11 + _m23 * ret._m12);
		ret._m20 = (_m10 * _m21 - _m20 * _m11) * invDet;
		ret._m21 = -(_m00 * _m21 - _m20 * _m01) * invDet;
		ret._m22 = (_m00 * _m11 - _m10 * _m01) * invDet;
		ret._m23 = -(_m03 * ret._m20 + _m13 * ret._m21 + _m23 * ret._m22);

		return ret;
	}

	STRING Matrix3x4::ToString() const
	{
		char tempBuffer[KhSTL::MATRIX_CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20, _m21, _m22,
			_m23);
		return STRING(tempBuffer);
	}

}
