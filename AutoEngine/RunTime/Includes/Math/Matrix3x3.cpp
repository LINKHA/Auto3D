#include "Matrix3x3.h"



MATH_BEGIN
Matrix3x3::Matrix3x3(const Matrix4x4 & other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];

	m_Data[3] = other.m_Data[4];
	m_Data[4] = other.m_Data[5];
	m_Data[5] = other.m_Data[6];

	m_Data[6] = other.m_Data[8];
	m_Data[7] = other.m_Data[9];
	m_Data[8] = other.m_Data[10];
}

Matrix3x3& Matrix3x3::SetZero()
{
	Get(0, 0) = 0.0f;	Get(0, 1) = 0.0f;	Get(0, 2) = 0.0f;
	Get(1, 0) = 0.0f;	Get(1, 1) = 0.0f;	Get(1, 2) = 0.0f;
	Get(2, 0) = 0.0f;	Get(2, 1) = 0.0f;	Get(2, 2) = 0.0f;
	return *this;
}

Matrix3x3& Matrix3x3::SetIdentity()
{
	Get(0, 0) = 1.0f;	Get(0, 1) = 0.0f;	Get(0, 2) = 0.0f;
	Get(1, 0) = 0.0f;	Get(1, 1) = 1.0f;	Get(1, 2) = 0.0f;
	Get(2, 0) = 0.0f;	Get(2, 1) = 0.0f;	Get(2, 2) = 1.0f;
	return *this;
}

Matrix3x3 & Matrix3x3::SetOrthoNormalBasis(const Vector3 & inX, const Vector3 & inY, const Vector3 & inZ)
{
	Get(0, 0) = inX[0];	Get(0, 1) = inY[0];	Get(0, 2) = inZ[0];
	Get(1, 0) = inX[1];	Get(1, 1) = inY[1];	Get(1, 2) = inZ[1];
	Get(2, 0) = inX[2];	Get(2, 1) = inY[2];	Get(2, 2) = inZ[2];
	return *this;
}

Matrix3x3 & Matrix3x3::SetOrthoNormalBasisInverse(const Vector3 & inX, const Vector3 & inY, const Vector3 & inZ)
{
	Get(0, 0) = inX[0];	Get(1, 0) = inY[0];	Get(2, 0) = inZ[0];
	Get(0, 1) = inX[1];	Get(1, 1) = inY[1];	Get(2, 1) = inZ[1];
	Get(0, 2) = inX[2];	Get(1, 2) = inY[2];	Get(2, 2) = inZ[2];
	return *this;
}


Matrix3x3 & Matrix3x3::operator=(const Matrix4x4 & other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];

	m_Data[3] = other.m_Data[4];
	m_Data[4] = other.m_Data[5];
	m_Data[5] = other.m_Data[6];

	m_Data[6] = other.m_Data[8];
	m_Data[7] = other.m_Data[9];
	m_Data[8] = other.m_Data[10];
	return *this;
}

Matrix3x3& Matrix3x3::operator *= (float f)
{
	for (int i = 0; i < 9; i++)
		m_Data[i] *= f;
	return *this;
}
Matrix3x3& Matrix3x3::operator *= (const Matrix3x3& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}

Matrix3x3& Matrix3x3::operator *= (const Matrix4x4& inM)
{
	int i;
	for (i = 0; i < 3; i++)
	{
		float v[3] = { Get(i, 0), Get(i, 1), Get(i, 2) };
		Get(i, 0) = v[0] * inM.Get(0, 0) + v[1] * inM.Get(1, 0) + v[2] * inM.Get(2, 0);
		Get(i, 1) = v[0] * inM.Get(0, 1) + v[1] * inM.Get(1, 1) + v[2] * inM.Get(2, 1);
		Get(i, 2) = v[0] * inM.Get(0, 2) + v[1] * inM.Get(1, 2) + v[2] * inM.Get(2, 2);
	}
	return *this;
}
MATH_END
