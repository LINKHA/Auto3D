#pragma once

#include "Quaternion.h"
#include "Vector4.h"

namespace Auto3D
{

template<typename _Ty> class TMatrix3x4;

/// 4x4 matrix for arbitrary linear transforms including projection.
template<typename _Ty> class AUTO_API TMatrix4x4
{
public:
	/// Matrix values.
	_Ty _m00, _m01, _m02, _m03;
	_Ty _m10, _m11, _m12, _m13;
	_Ty _m20, _m21, _m22, _m23;
	_Ty _m30, _m31, _m32, _m33;

	/// Construct an identity matrix
	///|1,0,0,0|
	///|0,1,0,0|
	///|0,0,1,0|
	///|0,0,0,1|
	TMatrix4x4() noexcept
		:_m00(1),
		_m01(0),
		_m02(0),
		_m03(0),
		_m10(0),
		_m11(1),
		_m12(0),
		_m13(0),
		_m20(0),
		_m21(0),
		_m22(1),
		_m23(0),
		_m30(0),
		_m31(0),
		_m32(0),
		_m33(1)
	{
	}

	/// Copy-construct.
	TMatrix4x4(const TMatrix4x4& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(matrix._m03),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(matrix._m13),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(matrix._m23),
		_m30(matrix._m30), _m31(matrix._m31), _m32(matrix._m32), _m33(matrix._m33)
	{
	}

	/// Copy-cnstruct from a 3x3 matrix and set the extra elements to identity.
	TMatrix4x4(const TMatrix3x3<_Ty>& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(0.0f),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(0.0f),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(0.0f),
		_m30(0.0f), _m31(0.0f), _m32(0.0f), _m33(1.0f)
	{
	}

	/// Copy-cnstruct from a 3x4 matrix and set the extra elements to identity.
	TMatrix4x4(const TMatrix3x4<_Ty>& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(matrix._m03),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(matrix._m13),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(matrix._m23),
		_m30(0.0f), _m31(0.0f), _m32(0.0f), _m33(1.0f)
	{
	}

	/// Construct from values.
	TMatrix4x4(_Ty v00, _Ty v01, _Ty v02, _Ty v03,
		_Ty v10, _Ty v11, _Ty v12, _Ty v13,
		_Ty v20, _Ty v21, _Ty v22, _Ty v23,
		_Ty v30, _Ty v31, _Ty v32, _Ty v33) :
		_m00(v00), _m01(v01), _m02(v02), _m03(v03),
		_m10(v10), _m11(v11), _m12(v12), _m13(v13),
		_m20(v20), _m21(v21), _m22(v22), _m23(v23),
		_m30(v30), _m31(v31), _m32(v32), _m33(v33)
	{
	}

	/// Construct from a _Ty array.
	TMatrix4x4(const _Ty* _data) :
		_m00(_data[0]), _m01(_data[1]), _m02(_data[2]), _m03(_data[3]),
		_m10(_data[4]), _m11(_data[5]), _m12(_data[6]), _m13(_data[7]),
		_m20(_data[8]), _m21(_data[9]), _m22(_data[10]), _m23(_data[11]),
		_m30(_data[12]), _m31(_data[13]), _m32(_data[14]), _m33(_data[15])
	{
	}

	/// Construct by parsing a string.
	TMatrix4x4(const FString& str)
	{
		FromString(str);
	}

		/// Assign from another matrix.
	TMatrix4x4& operator = (const TMatrix4x4& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02; _m03 = rhs._m03;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13;
		_m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23;
		_m30 = rhs._m30; _m31 = rhs._m31; _m32 = rhs._m32; _m33 = rhs._m33;
		return *this;
	}

	/// Assign from a 3x3 matrix. Set the extra elements to identity.
	TMatrix4x4& operator = (const TMatrix3x3<_Ty>& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02; _m03 = 0.0f;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12; _m13 = 0.0f;
		_m20 = rhs._m20; _m21 = rhs._m21;  _m22 = rhs._m22; _m23 = 0.0f;
		_m30 = 0.0f; _m31 = 0.0f; _m32 = 0.0f; _m33 = 1.0f;
		return *this;
	}

	/// Test for equality with another matrix without epsilon.
	bool operator == (const TMatrix4x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 16; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/// Test for inequality with another matrix without epsilon.
	bool operator != (const TMatrix4x4& rhs) const { return !(*this == rhs); }

	/// Multiply a TVector3 which is assumed to represent _position.
	TVector3<_Ty> operator * (const TVector3<_Ty>& rhs) const
	{
		_Ty invW = 1.0f / (_m30 * rhs._x + _m31 * rhs._y + _m32 * rhs._z + _m33);

		return TVector3<_Ty>(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03) * invW,
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13) * invW,
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23) * invW
			);
	}

	/// Multiply a TVector4.
	TVector4<_Ty> operator * (const TVector4<_Ty>& rhs) const
	{
		return TVector4<_Ty>(
			_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03 * rhs._w,
			_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13 * rhs._w,
			_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23 * rhs._w,
			_m30 * rhs._x + _m31 * rhs._y + _m32 * rhs._z + _m33 * rhs._w
			);
	}

	/// Add a matrix.
	TMatrix4x4 operator + (const TMatrix4x4& rhs) const
	{
		return TMatrix4x4(
			_m00 + rhs._m00, _m01 + rhs._m01, _m02 + rhs._m02, _m03 + rhs._m03,
			_m10 + rhs._m10, _m11 + rhs._m11, _m12 + rhs._m12, _m13 + rhs._m13,
			_m20 + rhs._m20, _m21 + rhs._m21, _m22 + rhs._m22, _m23 + rhs._m23,
			_m30 + rhs._m30, _m31 + rhs._m31, _m32 + rhs._m32, _m33 + rhs._m33
		);
	}

	/// Subtract a matrix.
	TMatrix4x4 operator - (const TMatrix4x4& rhs) const
	{
		return TMatrix4x4(
			_m00 - rhs._m00, _m01 - rhs._m01, _m02 - rhs._m02, _m03 - rhs._m03,
			_m10 - rhs._m10, _m11 - rhs._m11, _m12 - rhs._m12, _m13 - rhs._m13,
			_m20 - rhs._m20, _m21 - rhs._m21, _m22 - rhs._m22, _m23 - rhs._m23,
			_m30 - rhs._m30, _m31 - rhs._m31, _m32 - rhs._m32, _m33 - rhs._m33
		);
	}

	/// Multiply with a scalar.
	TMatrix4x4 operator * (_Ty rhs) const
	{
		return TMatrix4x4(
			_m00 * rhs, _m01 * rhs, _m02 * rhs, _m03 * rhs,
			_m10 * rhs, _m11 * rhs, _m12 * rhs, _m13 * rhs,
			_m20 * rhs, _m21 * rhs, _m22 * rhs, _m23 * rhs,
			_m30 * rhs, _m31 * rhs, _m32 * rhs, _m33 * rhs
		);
	}

	/// Multiply a matrix.
	TMatrix4x4 operator * (const TMatrix4x4& rhs) const
	{
		return TMatrix4x4(
			_m00 * rhs._m00 + _m01 * rhs._m10 + _m02 * rhs._m20 + _m03 * rhs._m30,
			_m00 * rhs._m01 + _m01 * rhs._m11 + _m02 * rhs._m21 + _m03 * rhs._m31,
			_m00 * rhs._m02 + _m01 * rhs._m12 + _m02 * rhs._m22 + _m03 * rhs._m32,
			_m00 * rhs._m03 + _m01 * rhs._m13 + _m02 * rhs._m23 + _m03 * rhs._m33,
			_m10 * rhs._m00 + _m11 * rhs._m10 + _m12 * rhs._m20 + _m13 * rhs._m30,
			_m10 * rhs._m01 + _m11 * rhs._m11 + _m12 * rhs._m21 + _m13 * rhs._m31,
			_m10 * rhs._m02 + _m11 * rhs._m12 + _m12 * rhs._m22 + _m13 * rhs._m32,
			_m10 * rhs._m03 + _m11 * rhs._m13 + _m12 * rhs._m23 + _m13 * rhs._m33,
			_m20 * rhs._m00 + _m21 * rhs._m10 + _m22 * rhs._m20 + _m23 * rhs._m30,
			_m20 * rhs._m01 + _m21 * rhs._m11 + _m22 * rhs._m21 + _m23 * rhs._m31,
			_m20 * rhs._m02 + _m21 * rhs._m12 + _m22 * rhs._m22 + _m23 * rhs._m32,
			_m20 * rhs._m03 + _m21 * rhs._m13 + _m22 * rhs._m23 + _m23 * rhs._m33,
			_m30 * rhs._m00 + _m31 * rhs._m10 + _m32 * rhs._m20 + _m33 * rhs._m30,
			_m30 * rhs._m01 + _m31 * rhs._m11 + _m32 * rhs._m21 + _m33 * rhs._m31,
			_m30 * rhs._m02 + _m31 * rhs._m12 + _m32 * rhs._m22 + _m33 * rhs._m32,
			_m30 * rhs._m03 + _m31 * rhs._m13 + _m32 * rhs._m23 + _m33 * rhs._m33
		);
	}

	/// Set translation elements.
	void SetTranslation(const TVector3<_Ty>& translation)
	{
		_m03 = translation._x;
		_m13 = translation._y;
		_m23 = translation._z;
	}

	/// Set rotation elements from a 3x3 matrix.
	void SetRotation(const TMatrix3x3<_Ty>& rotation)
	{
		_m00 = rotation._m00; _m01 = rotation._m01; _m02 = rotation._m02;
		_m10 = rotation._m10; _m11 = rotation._m11; _m12 = rotation._m12;
		_m20 = rotation._m20; _m21 = rotation._m21; _m22 = rotation._m22;
	}

	// Set scaling elements.
	void SetScale(const TVector3<_Ty>& scale)
	{
		_m00 = scale._x;
		_m11 = scale._y;
		_m22 = scale._z;
	}

	// Set uniform scaling elements.
	void SetScale(_Ty scale)
	{
		_m00 = scale;
		_m11 = scale;
		_m22 = scale;
	}

	/// Parse from a string. Return true on success.
	bool FromString(const FString& str)
	{
		return FromString(str.CString());
	}
	/// Parse from a C string. Return true on success.
	bool FromString(const char* str)
	{
		size_t elements = FString::CountElements(str, ' ');
		if (elements < 16)
			return false;

		char* ptr = (char*)str;
		_m00 = (_Ty)strtod(ptr, &ptr);
		_m01 = (_Ty)strtod(ptr, &ptr);
		_m02 = (_Ty)strtod(ptr, &ptr);
		_m03 = (_Ty)strtod(ptr, &ptr);
		_m10 = (_Ty)strtod(ptr, &ptr);
		_m11 = (_Ty)strtod(ptr, &ptr);
		_m12 = (_Ty)strtod(ptr, &ptr);
		_m13 = (_Ty)strtod(ptr, &ptr);
		_m20 = (_Ty)strtod(ptr, &ptr);
		_m21 = (_Ty)strtod(ptr, &ptr);
		_m22 = (_Ty)strtod(ptr, &ptr);
		_m23 = (_Ty)strtod(ptr, &ptr);
		_m30 = (_Ty)strtod(ptr, &ptr);
		_m31 = (_Ty)strtod(ptr, &ptr);
		_m32 = (_Ty)strtod(ptr, &ptr);
		_m33 = (_Ty)strtod(ptr, &ptr);

		return true;
	}

	/// Return the combined rotation and scaling matrix.
	TMatrix3x3<_Ty> ToMatrix3() const
	{
		return TMatrix3x3<_Ty>(
			_m00, _m01, _m02,
			_m10, _m11, _m12,
			_m20, _m21, _m22
			);
	}

	/// Return the rotation matrix with scaling removed.
	TMatrix3x3<_Ty> RotationMatrix() const
	{
		TVector3<_Ty> invScale(
			1.0f / sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			1.0f / sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			1.0f / sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
		);

		return ToMatrix3().Scaled(invScale);
	}

	/// Return the translation part.
	TVector3<_Ty> Translation() const
	{
		return TVector3<_Ty>(
			_m03,
			_m13,
			_m23
			);
	}

	/// Return the rotation part.
	FQuaternion Rotation() const { return FQuaternion(RotationMatrix()); }

	/// Return the scaling part.
	TVector3<_Ty> Scale() const
	{
		return TVector3<_Ty>(
			sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
			);
	}

	/// Return transpose.
	TMatrix4x4 Transpose() const
	{
		return TMatrix4x4(
			_m00, _m10, _m20, _m30,
			_m01, _m11, _m21, _m31,
			_m02, _m12, _m22, _m32,
			_m03, _m13, _m23, _m33
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const TMatrix4x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 16; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return decomposition to translation, rotation and scale.
	void Decompose(TVector3<_Ty>& translation, FQuaternion& rotation, TVector3<_Ty>& scale) const
	{
		translation._x = _m03;
		translation._y = _m13;
		translation._z = _m23;

		scale._x = sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20);
		scale._y = sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21);
		scale._z = sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22);

		TVector3<_Ty> invScale(1.0f / scale._x, 1.0f / scale._y, 1.0f / scale._z);
		rotation = FQuaternion(ToMatrix3().Scaled(invScale));
	}
	/// Return inverse.
	TMatrix4x4 Inverse() const
	{
		_Ty v0 = _m20 * _m31 - _m21 * _m30;
		_Ty v1 = _m20 * _m32 - _m22 * _m30;
		_Ty v2 = _m20 * _m33 - _m23 * _m30;
		_Ty v3 = _m21 * _m32 - _m22 * _m31;
		_Ty v4 = _m21 * _m33 - _m23 * _m31;
		_Ty v5 = _m22 * _m33 - _m23 * _m32;

		_Ty i00 = (v5 * _m11 - v4 * _m12 + v3 * _m13);
		_Ty i10 = -(v5 * _m10 - v2 * _m12 + v1 * _m13);
		_Ty i20 = (v4 * _m10 - v2 * _m11 + v0 * _m13);
		_Ty i30 = -(v3 * _m10 - v1 * _m11 + v0 * _m12);

		_Ty invDet = 1.0f / (i00 * _m00 + i10 * _m01 + i20 * _m02 + i30 * _m03);

		i00 *= invDet;
		i10 *= invDet;
		i20 *= invDet;
		i30 *= invDet;

		_Ty i01 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
		_Ty i11 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
		_Ty i21 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
		_Ty i31 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

		v0 = _m10 * _m31 - _m11 * _m30;
		v1 = _m10 * _m32 - _m12 * _m30;
		v2 = _m10 * _m33 - _m13 * _m30;
		v3 = _m11 * _m32 - _m12 * _m31;
		v4 = _m11 * _m33 - _m13 * _m31;
		v5 = _m12 * _m33 - _m13 * _m32;

		_Ty i02 = (v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
		_Ty i12 = -(v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
		_Ty i22 = (v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
		_Ty i32 = -(v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

		v0 = _m21 * _m10 - _m20 * _m11;
		v1 = _m22 * _m10 - _m20 * _m12;
		v2 = _m23 * _m10 - _m20 * _m13;
		v3 = _m22 * _m11 - _m21 * _m12;
		v4 = _m23 * _m11 - _m21 * _m13;
		v5 = _m23 * _m12 - _m22 * _m13;

		_Ty i03 = -(v5 * _m01 - v4 * _m02 + v3 * _m03) * invDet;
		_Ty i13 = (v5 * _m00 - v2 * _m02 + v1 * _m03) * invDet;
		_Ty i23 = -(v4 * _m00 - v2 * _m01 + v0 * _m03) * invDet;
		_Ty i33 = (v3 * _m00 - v1 * _m01 + v0 * _m02) * invDet;

		return TMatrix4x4<_Ty>(
			i00, i01, i02, i03,
			i10, i11, i12, i13,
			i20, i21, i22, i23,
			i30, i31, i32, i33
			);
	}
	/// Return _Ty data
	const _Ty* Data() const { return &_m00; }
	/// Return as string.
	FString ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20,
			_m21, _m22, _m23, _m30, _m31, _m32, _m33);
		return FString(tempBuffer);
	}

	/// Zero matrix.
	static const TMatrix4x4 ZERO;
	/// Identity matrix.
	static const TMatrix4x4 IDENTITY;
};

/// Construct by parsing a C string.
TMatrix4x4<char>::TMatrix4x4(const char* str)
{
	FromString(str);
}

/// Multiply a 4x4 matrix with a scalar
template <typename _Ty> TMatrix4x4<_Ty> operator * (_Ty lhs, const TMatrix4x4<_Ty>& rhs) { return rhs * lhs; }

using TMatrix4x4F = TMatrix4x4<float>;

using TMatrix4x4I = TMatrix4x4<int>;

using TMatrix4x4C = TMatrix4x4<char>;

using TMatrix4x4D = TMatrix4x4<double>;

using TMatrix4x4U = TMatrix4x4<unsigned>;


template <typename _Ty> TMatrix4x4<_Ty> LookAt(const TVector3<_Ty>& eye, const TVector3<_Ty>& center, const TVector3<_Ty>& up)
{
	TVector3<_Ty> f(center - eye);
	f.Normalize();
	TVector3<_Ty> s(Cross(f, up));
	s.Normalize();
	TVector3<_Ty> u(Cross(s, f));

	TMatrix4x4<_Ty> result;
	result._m00 = s._x;
	result._m10 = s._y;
	result._m20 = s._z;
	result._m01 = u._x;
	result._m11 = u._y;
	result._m21 = u._z;
	result._m02 = -f._x;
	result._m12 = -f._y;
	result._m22 = -f._z;
	result._m30 = -Dot(s, eye);
	result._m31 = -Dot(u, eye);
	result._m32 = Dot(f, eye);
	return result;
}


template <typename _Ty> TMatrix4x4<_Ty> Perspective(const _Ty& fovy, const _Ty& aspect, const _Ty& zNear, const _Ty& zFar)
{

	const _Ty rad = fovy * _Ty(0.01745329251994329576923690768489);
	_Ty tanHalfFovy = Tan(rad / static_cast<_Ty>(2));

	TMatrix4x4<_Ty> result;
	result._m00 = static_cast<_Ty>(1) / (aspect * tanHalfFovy);
	result._m11 = static_cast<_Ty>(1) / (tanHalfFovy);
	result._m22 = -(zFar + zNear) / (zFar - zNear);
	result._m23 = -static_cast<_Ty>(1);
	result._m32 = -(static_cast<_Ty>(2) * zFar * zNear) / (zFar - zNear);
	result._m33 = 0;
	return result;
}

}
