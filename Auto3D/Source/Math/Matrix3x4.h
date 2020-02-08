#pragma once

#include "Matrix4x4.h"

namespace Auto3D
{

/// 3x4 matrix for scene node transform calculations.
template<typename _Ty> class AUTO_API TMatrix3x4
{
public:
	/// Matrix values.
	_Ty _m00, _m01, _m02, _m03;
	_Ty _m10, _m11, _m12, _m13;
	_Ty _m20, _m21, _m22, _m23;
	/// Construct an identity matrix
	///|1,0,0,0|
	///|0,1,0,0|
	///|0,0,1,0|
	TMatrix3x4() noexcept
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
		_m23(0)
	{
	}

	/// Copy-construct.
	TMatrix3x4(const TMatrix3x4& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(matrix._m03),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(matrix._m13),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(matrix._m23)
	{
	}

	/// Copy-construct from a 3x3 matrix and set the extra elements to identity.
	TMatrix3x4(const TMatrix3x3<_Ty>& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(0.0f),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(0.0f),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(0.0f)
	{
	}

	/// Copy-construct from a 4x4 matrix which is assumed to contain no projection.
	TMatrix3x4(const TMatrix4x4<_Ty>& matrix) :
		_m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02), _m03(matrix._m03),
		_m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12), _m13(matrix._m13),
		_m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22), _m23(matrix._m23)
	{
	}

	// Construct from values.
	TMatrix3x4(_Ty v00, _Ty v01, _Ty v02, _Ty v03,
		_Ty v10, _Ty v11, _Ty v12, _Ty v13,
		_Ty v20, _Ty v21, _Ty v22, _Ty v23) :
		_m00(v00), _m01(v01), _m02(v02), _m03(v03),
		_m10(v10), _m11(v11), _m12(v12), _m13(v13),
		_m20(v20), _m21(v21), _m22(v22), _m23(v23)
	{
	}

	/// Construct from a _Ty array.
	TMatrix3x4(const _Ty* _data) :
		_m00(_data[0]), _m01(_data[1]), _m02(_data[2]), _m03(_data[3]),
		_m10(_data[4]), _m11(_data[5]), _m12(_data[6]), _m13(_data[7]),
		_m20(_data[8]), _m21(_data[9]), _m22(_data[10]), _m23(_data[11])
	{
	}

	/// Construct from translation, rotation and uniform scale.
	TMatrix3x4(const TVector3<_Ty>& translation, const FQuaternion& rotation, _Ty scale)
	{
		SetRotation(rotation.RotationMatrix() * scale);
		SetTranslation(translation);
	}
	/// Construct from translation, rotation and nonuniform scale.
	TMatrix3x4(const TVector3<_Ty>& translation, const FQuaternion& rotation, const TVector3<_Ty>& scale)
	{
		SetRotation(rotation.RotationMatrix().Scaled(scale));
		SetTranslation(translation);
	}
	/// Construct by parsing a string.
	TMatrix3x4(const FString& str)
	{
		FromString(str);
	}

	/// Assign from another matrix.
	TMatrix3x4& operator = (const TMatrix3x4& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02; _m03 = rhs._m03;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13;
		_m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23;
		return *this;
	}

	/// Assign from a 3x3 matrix and set the extra elements to identity.
	TMatrix3x4& operator = (const TMatrix3x3<_Ty>& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02; _m03 = 0.0;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12; _m13 = 0.0;
		_m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22; _m23 = 0.0;
		return *this;
	}

	/// Assign from a 4x4 matrix which is assumed to contain no projection.
	TMatrix3x4& operator = (const TMatrix4x4<_Ty>& rhs)
	{
		_m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02; _m03 = rhs._m03;
		_m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12; _m13 = rhs._m13;
		_m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22; _m23 = rhs._m23;
		return *this;
	}

	/// Test for equality with another matrix without epsilon.
	bool operator == (const TMatrix3x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 12; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/// Test for inequality with another matrix without epsilon.
	bool operator != (const TMatrix3x4& rhs) const { return !(*this == rhs); }

	/// Multiply a TVector3 which is assumed to represent _position.
	TVector3<_Ty> operator * (const TVector3<_Ty>& rhs) const
	{
		return TVector3<_Ty>(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03),
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13),
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23)
			);
	}

	/// Multiply a TVector4.
	TVector3<_Ty> operator * (const TVector4<_Ty>& rhs) const
	{
		return TVector3<_Ty>(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03 * rhs._w),
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13 * rhs._w),
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23 * rhs._w)
			);
	}

	/// Add a matrix.
	TMatrix3x4 operator + (const TMatrix3x4& rhs) const
	{
		return TMatrix3x4(
			_m00 + rhs._m00, _m01 + rhs._m01, _m02 + rhs._m02, _m03 + rhs._m03,
			_m10 + rhs._m10, _m11 + rhs._m11, _m12 + rhs._m12, _m13 + rhs._m13,
			_m20 + rhs._m20, _m21 + rhs._m21, _m22 + rhs._m22, _m23 + rhs._m23
		);
	}

	/// Subtract a matrix.
	TMatrix3x4 operator - (const TMatrix3x4& rhs) const
	{
		return TMatrix3x4(
			_m00 - rhs._m00, _m01 - rhs._m01, _m02 - rhs._m02, _m03 - rhs._m03,
			_m10 - rhs._m10, _m11 - rhs._m11, _m12 - rhs._m12, _m13 - rhs._m13,
			_m20 - rhs._m20, _m21 - rhs._m21, _m22 - rhs._m22, _m23 - rhs._m23
		);
	}

	/// Multiply with a scalar.
	TMatrix3x4 operator * (_Ty rhs) const
	{
		return TMatrix3x4(
			_m00 * rhs, _m01 * rhs, _m02 * rhs, _m03 * rhs,
			_m10 * rhs, _m11 * rhs, _m12 * rhs, _m13 * rhs,
			_m20 * rhs, _m21 * rhs, _m22 * rhs, _m23 * rhs
		);
	}

	/// Multiply a matrix.
	TMatrix3x4 operator * (const TMatrix3x4& rhs) const
	{
		return TMatrix3x4(
			_m00 * rhs._m00 + _m01 * rhs._m10 + _m02 * rhs._m20,
			_m00 * rhs._m01 + _m01 * rhs._m11 + _m02 * rhs._m21,
			_m00 * rhs._m02 + _m01 * rhs._m12 + _m02 * rhs._m22,
			_m00 * rhs._m03 + _m01 * rhs._m13 + _m02 * rhs._m23 + _m03,
			_m10 * rhs._m00 + _m11 * rhs._m10 + _m12 * rhs._m20,
			_m10 * rhs._m01 + _m11 * rhs._m11 + _m12 * rhs._m21,
			_m10 * rhs._m02 + _m11 * rhs._m12 + _m12 * rhs._m22,
			_m10 * rhs._m03 + _m11 * rhs._m13 + _m12 * rhs._m23 + _m13,
			_m20 * rhs._m00 + _m21 * rhs._m10 + _m22 * rhs._m20,
			_m20 * rhs._m01 + _m21 * rhs._m11 + _m22 * rhs._m21,
			_m20 * rhs._m02 + _m21 * rhs._m12 + _m22 * rhs._m22,
			_m20 * rhs._m03 + _m21 * rhs._m13 + _m22 * rhs._m23 + _m23
		);
	}

	/// Multiply a 4x4 matrix.
	TMatrix4x4<_Ty> operator * (const TMatrix4x4<_Ty>& rhs) const
	{
		return TMatrix4x4<_Ty>(
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
			rhs._m30,
			rhs._m31,
			rhs._m32,
			rhs._m33
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

	/// Set scaling elements.
	void SetScale(const TVector3<_Ty>& scale)
	{
		_m00 = scale._x;
		_m11 = scale._y;
		_m22 = scale._z;
	}

	/// Set uniform scaling elements.
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
		if (elements < 12)
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

	/// Convert to a 4x4 matrix by filling in an identity last row.
	TMatrix4x4<_Ty> ToMatrix4() const
	{
		return TMatrix4x4<_Ty>(
			_m00, _m01, _m02, _m03,
			_m10, _m11, _m12, _m13,
			_m20, _m21, _m22, _m23,
			0.0f, 0.0f, 0.0f, 1.0f
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

	/// Test for equality with another matrix with epsilon.
	bool Equals(const TMatrix3x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (size_t i = 0; i < 12; ++i)
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

		TVector3 invScale(1.0f / scale._x, 1.0f / scale._y, 1.0f / scale._z);
		rotation = QuaternionF(ToMatrix3().Scaled(invScale));
	}
	/// Return inverse.
	TMatrix3x4<_Ty> Inverse() const
	{
		_Ty det = _m00 * _m11 * _m22 +
			_m10 * _m21 * _m02 +
			_m20 * _m01 * _m12 -
			_m20 * _m11 * _m02 -
			_m10 * _m01 * _m22 -
			_m00 * _m21 * _m12;

		_Ty invDet = 1.0f / det;
		TMatrix3x4<_Ty> ret;

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


	/// Return _Ty data.
	_Ty* Data() { return &_m00; }
	/// Return as string.
	FString ToString() const
	{
		char tempBuffer[CONVERSION_BUFFER_LENGTH];
		sprintf(tempBuffer, "%g %g %g %g %g %g %g %g %g %g %g %g", _m00, _m01, _m02, _m03, _m10, _m11, _m12, _m13, _m20, _m21, _m22,
			_m23);
		return FString(tempBuffer);
	}

	/// Zero matrix.
	static const TMatrix3x4 ZERO;
	/// Identity matrix.
	static const TMatrix3x4 IDENTITY;
};

/// Construct by parsing a C string.
TMatrix3x4<char>::TMatrix3x4(const char* str)
{
	FromString(str);
}
	
/// Multiply a 3x4 matrix with a scalar.
template <typename _Ty> TMatrix3x4<_Ty> operator * (_Ty lhs, const TMatrix3x4<_Ty>& rhs) { return rhs * lhs; }

/// Multiply a 3x4 matrix with a 4x4 matrix.
template <typename _Ty> TMatrix4x4<_Ty> operator * (const TMatrix4x4<_Ty>& lhs, const TMatrix3x4<_Ty>& rhs)
{
	return TMatrix4x4<_Ty>(
		lhs._m00 * rhs._m00 + lhs._m01 * rhs._m10 + lhs._m02 * rhs._m20,
		lhs._m00 * rhs._m01 + lhs._m01 * rhs._m11 + lhs._m02 * rhs._m21,
		lhs._m00 * rhs._m02 + lhs._m01 * rhs._m12 + lhs._m02 * rhs._m22,
		lhs._m00 * rhs._m03 + lhs._m01 * rhs._m13 + lhs._m02 * rhs._m23 + lhs._m03,
		lhs._m10 * rhs._m00 + lhs._m11 * rhs._m10 + lhs._m12 * rhs._m20,
		lhs._m10 * rhs._m01 + lhs._m11 * rhs._m11 + lhs._m12 * rhs._m21,
		lhs._m10 * rhs._m02 + lhs._m11 * rhs._m12 + lhs._m12 * rhs._m22,
		lhs._m10 * rhs._m03 + lhs._m11 * rhs._m13 + lhs._m12 * rhs._m23 + lhs._m13,
		lhs._m20 * rhs._m00 + lhs._m21 * rhs._m10 + lhs._m22 * rhs._m20,
		lhs._m20 * rhs._m01 + lhs._m21 * rhs._m11 + lhs._m22 * rhs._m21,
		lhs._m20 * rhs._m02 + lhs._m21 * rhs._m12 + lhs._m22 * rhs._m22,
		lhs._m20 * rhs._m03 + lhs._m21 * rhs._m13 + lhs._m22 * rhs._m23 + lhs._m23,
		lhs._m30 * rhs._m00 + lhs._m31 * rhs._m10 + lhs._m32 * rhs._m20,
		lhs._m30 * rhs._m01 + lhs._m31 * rhs._m11 + lhs._m32 * rhs._m21,
		lhs._m30 * rhs._m02 + lhs._m31 * rhs._m12 + lhs._m32 * rhs._m22,
		lhs._m30 * rhs._m03 + lhs._m31 * rhs._m13 + lhs._m32 * rhs._m23 + lhs._m33
		);
}
using TMatrix3x4F = TMatrix3x4<float>;

using TMatrix3x4I = TMatrix3x4<int>;

using TMatrix3x4C = TMatrix3x4<char>;

using TMatrix3x4D = TMatrix3x4<double>;

using TMatrix3x4U = TMatrix3x4<unsigned>;

}
