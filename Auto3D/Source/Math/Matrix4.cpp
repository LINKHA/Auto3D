#include "../Base/String.h"
#include "Matrix4.h"

#include <cstdio>
#include <cstdlib>

#include "../Debug/DebugNew.h"

namespace Auto3D
{

const Matrix4x4F Matrix4x4F::ZERO(
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f);
const Matrix4x4F Matrix4x4F::IDENTITY;

const Matrix4x4I Matrix4x4I::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix4x4I Matrix4x4I::IDENTITY;

const Matrix4x4C Matrix4x4C::ZERO(
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0,
	0, 0, 0, 0);
const Matrix4x4C Matrix4x4C::IDENTITY;

const Matrix4x4D Matrix4x4D::ZERO(
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0,
	0.0, 0.0, 0.0, 0.0);
const Matrix4x4D Matrix4x4D::IDENTITY;


	//template<typename _Ty> bool Matrix4x4<_Ty>::FromString(const String& str)
	//{
	//	return FromString(str.CString());
	//}

	//template<typename _Ty> bool Matrix4x4<_Ty>::FromString(const char* str)
	//{
	//	size_t elements = String::CountElements(str, ' ');
	//	if (elements < 16)
	//		return false;

	//	char* ptr = (char*)str;
	//	_m00 = (_Ty)strtod(ptr, &ptr);
	//	_m01 = (_Ty)strtod(ptr, &ptr);
	//	_m02 = (_Ty)strtod(ptr, &ptr);
	//	_m03 = (_Ty)strtod(ptr, &ptr);
	//	_m10 = (_Ty)strtod(ptr, &ptr);
	//	_m11 = (_Ty)strtod(ptr, &ptr);
	//	_m12 = (_Ty)strtod(ptr, &ptr);
	//	_m13 = (_Ty)strtod(ptr, &ptr);
	//	_m20 = (_Ty)strtod(ptr, &ptr);
	//	_m21 = (_Ty)strtod(ptr, &ptr);
	//	_m22 = (_Ty)strtod(ptr, &ptr);
	//	_m23 = (_Ty)strtod(ptr, &ptr);
	//	_m30 = (_Ty)strtod(ptr, &ptr);
	//	_m31 = (_Ty)strtod(ptr, &ptr);
	//	_m32 = (_Ty)strtod(ptr, &ptr);
	//	_m33 = (_Ty)strtod(ptr, &ptr);

	//	return true;
	//}

	//template<typename _Ty> void Matrix4x4<_Ty>::Decompose(Vector3<_Ty>& translation, Quaternion<_Ty>& rotation, Vector3<_Ty>& scale) const
	//{
	//	translation._x = _m03;
	//	translation._y = _m13;
	//	translation._z = _m23;

	//	scale._x = sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20);
	//	scale._y = sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21);
	//	scale._z = sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22);

	//	Vector3<_Ty> invScale(1.0f / scale._x, 1.0f / scale._y, 1.0f / scale._z);
	//	rotation = Quaternion<_Ty>(ToMatrix3().Scaled(invScale));
	//}

	//template<typename _Ty> Matrix4x4<_Ty> Matrix4x4<_Ty>::Inverse() const
	//{
	//	_Ty v0 = _m20 * _m31 - _m21 * _m30;
	//	_Ty v1 = _m20 * _m32 - _m22 * _m30;
	//	_Ty v2 = _m20 * _m33 - _m23 * _m30;
	//	_Ty v3 = _m21 * _m32 - _m22 * _m31;
	//	_Ty v4 = _m21 * _m33 - _m23 * _m31;
	//	_Ty v5 = _m22 * _m33 - _m23 * _m32;

	//	_Ty i00 = (v5 * _m11 - v4 * _m12 + v3 * _m13);
	//	_Ty i10 = -(v5 * _m10 - v2 * _m12 + v1 * _m13);
	//	_Ty i20 = (v4 * _m10 - v2 * _m11 + v0 * _m13);
	//	_Ty i30 = -(v3 * _m10 - v1 * _m11 + v0 * _m12);

	//	_Ty invDet = 1.0f / (i00 * _m00 + i10 * _m01 + i20 * _m02 + i30 * _m03);

	//	i00 *= invDet;
	//	i10 *= invDet;
	//	i20 *= invDet;
	//	i30 *= invDet;

	//	_Ty i01 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
	//	_Ty i11 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
	//	_Ty i21 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
	//	_Ty i31 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

	//	v0 = _m10 * _m31 - _m11 * _m30;
	//	v1 = _m10 * _m32 - _m12 * _m30;
	//	v2 = _m10 * _m33 - _m13 * _m30;
	//	v3 = _m11 * _m32 - _m12 * _m31;
	//	v4 = _m11 * _m33 - _m13 * _m31;
	//	v5 = _m12 * _m33 - _m13 * _m32;

	//	_Ty i02 = (v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
	//	_Ty i12 = -(v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
	//	_Ty i22 = (v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
	//	_Ty i32 = -(v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

	//	v0 = _m21 * _m10 - _m20 * _m11;
	//	v1 = _m22 * _m10 - _m20 * _m12;
	//	v2 = _m23 * _m10 - _m20 * _m13;
	//	v3 = _m22 * _m11 - _m21 * _m12;
	//	v4 = _m23 * _m11 - _m21 * _m13;
	//	v5 = _m23 * _m12 - _m22 * _m13;

	//	_Ty i03 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
	//	_Ty i13 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
	//	_Ty i23 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
	//	_Ty i33 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

	//	return Matrix4x4<_Ty>(
	//		i00, i01, i02, i03,
	//		i10, i11, i12, i13,
	//		i20, i21, i22, i23,
	//		i30, i31, i32, i33
	//		);
	//}

	//template<typename _Ty> String Matrix4x4<_Ty>::ToString() const
	//{
	//	char tempBuffer[CONVERSION_BUFFER_LENGTH];
	//	sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20,
	//		_m21, _m22, _m23, _m30, _m31, _m32, _m33);
	//	return String(tempBuffer);
	//}

}
