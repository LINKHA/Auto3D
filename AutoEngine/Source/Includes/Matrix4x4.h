#pragma once
#include "MathBase.h"
#include "Vector3.h"
#include "Matrix3x3.h"
namespace Auto3D {

struct Vector3;
struct Vector4;
struct Quaternion;


struct Matrix4x4
{
public:
	float data[16];

	Matrix4x4() {}
	Matrix4x4(const struct Matrix3x3& m);
	Matrix4x4(const float data[16]);

	float& Get(int row, int column)					{ return data[row + (column * 4)]; }
	const float& Get(int row, int column)const		{ return data[row + (column * 4)]; }
	float* GetPtr()									{ return data; }
	const float* GetPtr() const						{ return data; }


	Matrix4x4& SetIdentity();
	Matrix4x4& SetPerspectiveCotan(float cotanHalfFOV, float zNear, float zFar);
	Matrix4x4& SetOrtho(float left, float right, float bottom, float top, float zNear, float zFar);
	Matrix4x4& SetFrustum(float left, float right, float bottom, float top, float nearval, float farval);
	Matrix4x4& SetTranslate(const Vector3& inTrans);
	Matrix4x4& SetOrthoNormalBasis(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix4x4& SetOrthoNormalBasisInverse(const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix4x4& SetScale(const Vector3& inScale);
	Matrix4x4& SetPositionAndOrthoNormalBasis(const Vector3& inPosition, const Vector3& inX, const Vector3& inY, const Vector3& inZ);
	Matrix4x4& Translate(const Vector3& inTrans);
	Matrix4x4& Scale(const Vector3& inScale);
	Matrix4x4& SetFromToRotation(const Vector3& from, const Vector3& to);

	void SetTR(const Vector3& pos, const Quaternion& q);
	void SetTRS(const Vector3& pos, const Quaternion& q, const Vector3& s);
	void SetTRInverse(const Vector3& pos, const Quaternion& q);

	Vector3& GetAxisX() const;
	Vector3& GetAxisY() const;
	Vector3& GetAxisZ() const;
	Vector3& GetPosition() const;
	Vector4& GetRow(int row) const;
	Vector4& GetColumn(int col) const;
	// these set only these components of the matrix, everything else is untouched!
	void SetAxisX(const Vector3& v);
	void SetAxisY(const Vector3& v);
	void SetAxisZ(const Vector3& v);
	void SetPosition(const Vector3& v);
	void SetRow(int row, const Vector4& v);
	void SetColumn(int col, const Vector4& v);

	bool IsIdentity(float epsilon = MATH_EPSILON) const;

	float& operator [] (int row)					{ return data[row]; }
	float operator [] (int row) const				{ return data[row]; }

	Matrix4x4& operator = (const struct Matrix3x3& m);
	Matrix4x4& operator *= (const Matrix4x4& inM);


	/* the  identity mat4x4
	|1,0,0,0|
	|0,1,0,0|
	|0,0,1,0|
	|0,0,0,1|
	*/
	static const glm::mat4 Matrix4x4::identity;
};

}


