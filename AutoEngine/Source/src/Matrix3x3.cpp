#include "Matrix3x3.h"

namespace Auto3D {
Matrix3x3::Matrix3x3(const Matrix4x4 & other)
{
	data[0] = other.data[0];
	data[1] = other.data[1];
	data[2] = other.data[2];

	data[3] = other.data[4];
	data[4] = other.data[5];
	data[5] = other.data[6];

	data[6] = other.data[8];
	data[7] = other.data[9];
	data[8] = other.data[10];
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
	data[0] = other.data[0];
	data[1] = other.data[1];
	data[2] = other.data[2];

	data[3] = other.data[4];
	data[4] = other.data[5];
	data[5] = other.data[6];

	data[6] = other.data[8];
	data[7] = other.data[9];
	data[8] = other.data[10];
	return *this;
}

Matrix3x3& Matrix3x3::operator *= (float f)
{
	for (int i = 0; i < 9; i++)
		data[i] *= f;
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
}
