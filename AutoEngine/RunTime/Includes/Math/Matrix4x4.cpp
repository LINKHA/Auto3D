#include "Matrix4x4.h"
AUTO_BEGIN

const glm::mat4 Matrix4x4::identity = glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);

Matrix4x4::Matrix4x4(const Matrix3x3 &other)
{
	data[0] = other.data[0];
	data[1] = other.data[1];
	data[2] = other.data[2];
	data[3] = 0.0f;

	data[4] = other.data[3];
	data[5] = other.data[4];
	data[6] = other.data[5];
	data[7] = 0.0f;

	data[8] = other.data[6];
	data[9] = other.data[7];
	data[10] = other.data[8];
	data[11] = 0.0f;

	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
}

Matrix4x4::Matrix4x4(const float data[16])
{
	for (int i = 0; i<16; i++)
		this->data[i] = data[i];
}

Matrix4x4& Matrix4x4::operator = (const Matrix3x3& other)
{
	data[0] = other.data[0];
	data[1] = other.data[1];
	data[2] = other.data[2];
	data[3] = 0.0f;

	data[4] = other.data[3];
	data[5] = other.data[4];
	data[6] = other.data[5];
	data[7] = 0.0f;

	data[8] = other.data[6];
	data[9] = other.data[7];
	data[10] = other.data[8];
	data[11] = 0.0f;

	data[12] = 0.0f;
	data[13] = 0.0f;
	data[14] = 0.0f;
	data[15] = 1.0f;
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
AUTO_END