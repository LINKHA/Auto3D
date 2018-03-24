#ifndef MATRIX_3X3_H_
#define MATRIX_3X3_H_
#include "AUMathBase.h"
#include "Vector3.h"
#include "Matrix4x4.h"
MATH_BEGIN
struct Matrix3x3
{
public:
	float m_Data[9];
	// The floats are laid out:
	// m0   m3   m6	
	// m1   m4   m7	
	// m2   m5   m8	
	Matrix3x3() {}
	Matrix3x3(float m00, float m01, float m02, float m10, float m11, float m12, float m20, float m21, float m22) { Get(0, 0) = m00; Get(1, 0) = m10; Get(2, 0) = m20; Get(0, 1) = m01; Get(1, 1) = m11; Get(2, 1) = m21; Get(0, 2) = m02; Get(1, 2) = m12; Get(2, 2) = m22; }
	Matrix3x3(const struct Matrix4x4& m);

	float& Get(int row, int column) { return m_Data[row + (column * 3)]; }
	const float& Get(int row, int column)const { return m_Data[row + (column * 3)]; }
	float* GetPtr() { return m_Data; }
	const float* GetPtr() const { return m_Data; }

	Matrix3x3& SetZero();
	Matrix3x3& SetIdentity();

	Matrix3x3& SetScale(const Vector3& inScale);
	Matrix3x3& Scale(const Vector3& inScale);
	Matrix3x3& SetFromToRotation(const Vector3& from, const Vector3& to);
	Matrix3x3& SetAxisAngle(const Vector3& rotationAxis, float radians);

	Matrix3x3& SetOrthoNormalBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix3x3& SetOrthoNormalBasisInverse(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	
	float& operator [] (int row) { return m_Data[row]; }
	float operator [] (int row) const { return m_Data[row]; }

	Matrix3x3& operator = (const struct Matrix4x4& m);
	Matrix3x3& operator *= (const Matrix3x3& inM);
	Matrix3x3& operator *= (const struct Matrix4x4& inM);

	friend Matrix3x3 operator * (const Matrix3x3& lhs, const Matrix3x3& rhs)	{ Matrix3x3 temp(lhs); temp *= rhs; return temp; }
	Matrix3x3& operator *= (float f);
	Matrix3x3& operator /= (float f)											{ return *this *= (1.0f / f); }


};

MATH_END
#endif //!MATRIX_3X3_H_
