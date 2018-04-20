#include "Matrix4x4.h"
MATH_BEGIN

const glm::mat4 Matrix4x4::identity = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

Matrix4x4::Matrix4x4(const Matrix3x3 &other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];
	m_Data[3] = 0.0f;

	m_Data[4] = other.m_Data[3];
	m_Data[5] = other.m_Data[4];
	m_Data[6] = other.m_Data[5];
	m_Data[7] = 0.0f;

	m_Data[8] = other.m_Data[6];
	m_Data[9] = other.m_Data[7];
	m_Data[10] = other.m_Data[8];
	m_Data[11] = 0.0f;

	m_Data[12] = 0.0f;
	m_Data[13] = 0.0f;
	m_Data[14] = 0.0f;
	m_Data[15] = 1.0f;
}

Matrix4x4::Matrix4x4(const float data[16])
{
	for (int i = 0; i<16; i++)
		m_Data[i] = data[i];
}

Matrix4x4& Matrix4x4::operator = (const Matrix3x3& other)
{
	m_Data[0] = other.m_Data[0];
	m_Data[1] = other.m_Data[1];
	m_Data[2] = other.m_Data[2];
	m_Data[3] = 0.0f;

	m_Data[4] = other.m_Data[3];
	m_Data[5] = other.m_Data[4];
	m_Data[6] = other.m_Data[5];
	m_Data[7] = 0.0f;

	m_Data[8] = other.m_Data[6];
	m_Data[9] = other.m_Data[7];
	m_Data[10] = other.m_Data[8];
	m_Data[11] = 0.0f;

	m_Data[12] = 0.0f;
	m_Data[13] = 0.0f;
	m_Data[14] = 0.0f;
	m_Data[15] = 1.0f;
	return *this;
}


Matrix4x4& Matrix4x4::operator *= (const Matrix4x4& inM1)
{
	assert(&inM1 != this);
	int i;
	for (i = 0; i < 4; i++)
	{
		float v[4] = { Get(i, 0), Get(i, 1), Get(i, 2), Get(i,3) };
		Get(i, 0) = v[0] * inM1.Get(0, 0) + v[1] * inM1.Get(1, 0) + v[2] * inM1.Get(2, 0) + inM1.Get(3, 0);
		Get(i, 1) = v[0] * inM1.Get(0, 1) + v[1] * inM1.Get(1, 1) + v[2] * inM1.Get(2, 1) + inM1.Get(3, 1);
		Get(i, 2) = v[0] * inM1.Get(0, 2) + v[1] * inM1.Get(1, 2) + v[2] * inM1.Get(2, 2) + inM1.Get(3, 2);
		Get(i, 3) = v[0] * inM1.Get(0, 3) + v[1] * inM1.Get(1, 3) + v[2] * inM1.Get(2, 3) + inM1.Get(3, 3);
	}
	return *this;

}
MATH_END