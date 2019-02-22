#pragma once

#include "Matrix4x4.h"

namespace Auto3D
{

/// 3x4 matrix for scene node transform calculations.

template<typename _Ty> class Matrix3x4
{
public:
	/// Construct an identity matrix.
	Matrix3x4() noexcept
		:_m00(1.0f),
		_m01(0.0f),
		_m02(0.0f),
		_m03(0.0f),
		_m10(0.0f),
		_m11(1.0f),
		_m12(0.0f),
		_m13(0.0f),
		_m20(0.0f),
		_m21(0.0f),
		_m22(1.0f),
		_m23(0.0f)
	{
	}

	/// Copy-construct from another matrix.
	Matrix3x4(const Matrix3x4& matrix) noexcept = default;

	/// Copy-construct from a 3x3 matrix and set the extra elements to identity.
	explicit Matrix3x4(const Matrix3x3<_Ty>& matrix) noexcept :
		_m00(matrix._m00),
		_m01(matrix._m01),
		_m02(matrix._m02),
		_m03(0.0f),
		_m10(matrix._m10),
		_m11(matrix._m11),
		_m12(matrix._m12),
		_m13(0.0f),
		_m20(matrix._m20),
		_m21(matrix._m21),
		_m22(matrix._m22),
		_m23(0.0f)
	{
	}

	/// Copy-construct from a 4x4 matrix which is assumed to contain no projection.
	explicit Matrix3x4(const Matrix4x4<_Ty>& matrix) noexcept
		:_m00(matrix._m00),
		_m01(matrix._m01),
		_m02(matrix._m02),
		_m03(matrix._m03),
		_m10(matrix._m10),
		_m11(matrix._m11),
		_m12(matrix._m12),
		_m13(matrix._m13),
		_m20(matrix._m20),
		_m21(matrix._m21),
		_m22(matrix._m22),
		_m23(matrix._m23)
	{
	}

	/// Construct from values.
	Matrix3x4(_Ty v00, _Ty v01, _Ty v02, _Ty v03,
		_Ty v10, _Ty v11, _Ty v12, _Ty v13,
		_Ty v20, _Ty v21, _Ty v22, _Ty v23) noexcept :
		_m00(v00),
		_m01(v01),
		_m02(v02),
		_m03(v03),
		_m10(v10),
		_m11(v11),
		_m12(v12),
		_m13(v13),
		_m20(v20),
		_m21(v21),
		_m22(v22),
		_m23(v23)
	{
	}

	/// Construct from a _Ty array.
	explicit Matrix3x4(const _Ty* data) noexcept
		:_m00(data[0]),
		_m01(data[1]),
		_m02(data[2]),
		_m03(data[3]),
		_m10(data[4]),
		_m11(data[5]),
		_m12(data[6]),
		_m13(data[7]),
		_m20(data[8]),
		_m21(data[9]),
		_m22(data[10]),
		_m23(data[11])
	{
	}

	/// Construct from translation, rotation and uniform scale.
	Matrix3x4(const Vector3<_Ty>& translation, const Quaternion<_Ty>& rotation, _Ty scale) noexcept
	{
		SetRotation(rotation.RotationMatrix() * scale);
		SetTranslation(translation);
	}

	/// Construct from translation, rotation and nonuniform scale.
	Matrix3x4(const Vector3<_Ty>& translation, const Quaternion<_Ty>& rotation, const Vector3<_Ty>& scale) noexcept
	{
		SetRotation(rotation.RotationMatrix().Scaled(scale));
		SetTranslation(translation);
	}

	/// Assign from another matrix.
	Matrix3x4& operator =(const Matrix3x4& rhs) noexcept = default;

	/// Assign from a 3x3 matrix and set the extra elements to identity.
	Matrix3x4& operator =(const Matrix3x3<_Ty>& rhs) noexcept
	{
		_m00 = rhs._m00;
		_m01 = rhs._m01;
		_m02 = rhs._m02;
		_m03 = 0.0f;
		_m10 = rhs._m10;
		_m11 = rhs._m11;
		_m12 = rhs._m12;
		_m13 = 0.0f;
		_m20 = rhs._m20;
		_m21 = rhs._m21;
		_m22 = rhs._m22;
		_m23 = 0.0f;
		return *this;
	}

	/// Assign from a 4x4 matrix which is assumed to contain no projection.
	Matrix3x4& operator =(const Matrix4x4<_Ty>& rhs) noexcept
	{
		_m00 = rhs._m00;
		_m01 = rhs._m01;
		_m02 = rhs._m02;
		_m03 = rhs._m03;
		_m10 = rhs._m10;
		_m11 = rhs._m11;
		_m12 = rhs._m12;
		_m13 = rhs._m13;
		_m20 = rhs._m20;
		_m21 = rhs._m21;
		_m22 = rhs._m22;
		_m23 = rhs._m23;

		return *this;
	}

	/// Test for equality with another matrix without epsilon.
	bool operator ==(const Matrix3x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (unsigned i = 0; i < 12; ++i)
		{
			if (leftData[i] != rightData[i])
				return false;
		}

		return true;
	}

	/// Test for inequality with another matrix without epsilon.
	bool operator !=(const Matrix3x4& rhs) const { return !(*this == rhs); }

	/// Multiply a Vector3 which is assumed to represent position.
	Vector3<_Ty> operator *(const Vector3<_Ty>& rhs) const
	{
		return Vector3<_Ty>(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03),
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13),
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23)
		);
	}

	/// Multiply a Vector4.
	Vector3<_Ty> operator *(const Vector4<_Ty>& rhs) const
	{
		return Vector3<_Ty>(
			(_m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z + _m03 * rhs._w),
			(_m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z + _m13 * rhs._w),
			(_m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z + _m23 * rhs._w)
		);
	}

	/// Add a matrix.
	Matrix3x4 operator +(const Matrix3x4& rhs) const
	{
		return Matrix3x4(
			_m00 + rhs._m00,
			_m01 + rhs._m01,
			_m02 + rhs._m02,
			_m03 + rhs._m03,
			_m10 + rhs._m10,
			_m11 + rhs._m11,
			_m12 + rhs._m12,
			_m13 + rhs._m13,
			_m20 + rhs._m20,
			_m21 + rhs._m21,
			_m22 + rhs._m22,
			_m23 + rhs._m23
		);
	}

	/// Subtract a matrix.
	Matrix3x4 operator -(const Matrix3x4& rhs) const
	{
		return Matrix3x4(
			_m00 - rhs._m00,
			_m01 - rhs._m01,
			_m02 - rhs._m02,
			_m03 - rhs._m03,
			_m10 - rhs._m10,
			_m11 - rhs._m11,
			_m12 - rhs._m12,
			_m13 - rhs._m13,
			_m20 - rhs._m20,
			_m21 - rhs._m21,
			_m22 - rhs._m22,
			_m23 - rhs._m23
		);
	}

	/// Multiply with a scalar.
	Matrix3x4 operator *(_Ty rhs) const
	{
		return Matrix3x4(
			_m00 * rhs,
			_m01 * rhs,
			_m02 * rhs,
			_m03 * rhs,
			_m10 * rhs,
			_m11 * rhs,
			_m12 * rhs,
			_m13 * rhs,
			_m20 * rhs,
			_m21 * rhs,
			_m22 * rhs,
			_m23 * rhs
		);
	}

	/// Multiply a matrix.
	Matrix3x4 operator *(const Matrix3x4& rhs) const
	{
		return Matrix3x4(
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
	Matrix4x4<_Ty> operator *(const Matrix4x4<_Ty>& rhs) const
	{
		return Matrix4x4<_Ty>(
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
	void SetTranslation(const Vector3<_Ty>& translation)
	{
		_m03 = translation._x;
		_m13 = translation._y;
		_m23 = translation._z;
	}

	/// Set rotation elements from a 3x3 matrix.
	void SetRotation(const Matrix3x3<_Ty>& rotation)
	{
		_m00 = rotation._m00;
		_m01 = rotation._m01;
		_m02 = rotation._m02;
		_m10 = rotation._m10;
		_m11 = rotation._m11;
		_m12 = rotation._m12;
		_m20 = rotation._m20;
		_m21 = rotation._m21;
		_m22 = rotation._m22;
	}

	/// Set scaling elements.
	void SetScale(const Vector3<_Ty>& scale)
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

	/// Return the combined rotation and scaling matrix.
	Matrix3x3<_Ty> ToMatrix3() const
	{
		return Matrix3x3<_Ty>(
			_m00,
			_m01,
			_m02,
			_m10,
			_m11,
			_m12,
			_m20,
			_m21,
			_m22
		);
	}

	/// Convert to a 4x4 matrix by filling in an identity last row.
	Matrix4x4<_Ty> ToMatrix4() const
	{
		return Matrix4x4<_Ty>(
			_m00,
			_m01,
			_m02,
			_m03,
			_m10,
			_m11,
			_m12,
			_m13,
			_m20,
			_m21,
			_m22,
			_m23,
			0.0f,
			0.0f,
			0.0f,
			1.0f
		);
	}

	/// Return the rotation matrix with scaling removed.
	Matrix3x3<_Ty> RotationMatrix() const
	{
		Vector3<_Ty> invScale(
			1.0f / sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			1.0f / sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			1.0f / sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
		);

		return ToMatrix3().Scaled(invScale);
	}

	/// Return the translation part.
	Vector3<_Ty> Translation() const
	{
		return Vector3<_Ty>(
			_m03,
			_m13,
			_m23
		);
	}

	/// Return the rotation part.
	Quaternion<_Ty> Rotation() const { return Quaternion<_Ty>(RotationMatrix()); }

	/// Return the scaling part.
	Vector3<_Ty> Scale() const
	{
		return Vector3<_Ty>(
			sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
			sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
			sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
		);
	}

	/// Return the scaling part with the sign. Reference rotation matrix is required to avoid ambiguity.
	Vector3<_Ty> SignedScale(const Matrix3x3<_Ty>& rotation) const
	{
		return Vector3<_Ty>(
			rotation._m00 * _m00 + rotation._m10 * _m10 + rotation._m20 * _m20,
			rotation._m01 * _m01 + rotation._m11 * _m11 + rotation._m21 * _m21,
			rotation._m02 * _m02 + rotation._m12 * _m12 + rotation._m22 * _m22
		);
	}

	/// Test for equality with another matrix with epsilon.
	bool Equals(const Matrix3x4& rhs) const
	{
		const _Ty* leftData = Data();
		const _Ty* rightData = rhs.Data();

		for (unsigned i = 0; i < 12; ++i)
		{
			if (!Auto3D::Equals(leftData[i], rightData[i]))
				return false;
		}

		return true;
	}

	/// Return decomposition to translation, rotation and scale.
	void Decompose(Vector3<_Ty>& translation, Quaternion<_Ty>& rotation, Vector3<_Ty>& scale) const;

	/// Return inverse.
	Matrix3x4 Inverse() const;

	/// Return _Ty data.
	const _Ty* Data() const { return &_m00; }

	/// Return matrix element.
	_Ty Element(unsigned i, unsigned j) const { return Data()[i * 4 + j]; }

	/// Return matrix row.
	Vector4<_Ty> Row(unsigned i) const { return Vector4<_Ty>(Element(i, 0), Element(i, 1), Element(i, 2), Element(i, 3)); }

	/// Return matrix column.
	Vector3<_Ty> Column(unsigned j) const { return Vector3<_Ty>(Element(0, j), Element(1, j), Element(2, j)); }

	/// Return as string.
	STRING ToString() const;

	_Ty _m00;
	_Ty _m01;
	_Ty _m02;
	_Ty _m03;
	_Ty _m10;
	_Ty _m11;
	_Ty _m12;
	_Ty _m13;
	_Ty _m20;
	_Ty _m21;
	_Ty _m22;
	_Ty _m23;

	/// Zero matrix.
	static const Matrix3x4 ZERO;
	/// Identity matrix.
	static const Matrix3x4 IDENTITY;
};

/// Multiply a 3x4 matrix with a scalar.
template <typename _Ty> Matrix3x4<_Ty> operator *(_Ty lhs, const Matrix3x4<_Ty>& rhs) { return rhs * lhs; }

using Matrix3x4F = Matrix3x4<float>;

using Matrix3x4I = Matrix3x4<int>;

using Matrix3x4C = Matrix3x4<char>;

using Matrix3x4D = Matrix3x4<double>;

using Matrix3x4U = Matrix3x4<unsigned>;

}
