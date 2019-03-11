#include "MathTransform.h"
#include "Matrix4x4.h"
#include "Vector3.h"
namespace Auto3D
{/*
template <typename _Ty> Matrix4x4<_Ty> LookAt(const Vector3<_Ty>& eye, const Vector3<_Ty>& center, const Vector3<_Ty>& up)
{
	Vector3<_Ty> f(center - eye);
	f.Normalize();
	Vector3<_Ty> s(Cross(f, up));
	s.Normalize();
	Vector3<_Ty> u(Auto3D::Cross(s, f));

	Matrix4x4<_Ty> result;
	result._m00 = s.x;
	result._m10 = s.y;
	result._m20 = s.z;
	result._m01 = u.x;
	result._m11 = u.y;
	result._m21 = u.z;
	result._m02 = -f.x;
	result._m12 = -f.y;
	result._m22 = -f.z;
	result._m30 = -Dot(s, eye);
	result._m31 = -Dot(u, eye);
	result._m32 = Dot(f, eye);
	return result;
}

template <typename _Ty> Matrix4x4<_Ty> Perspective(const _Ty& fovy, const _Ty& aspect, const _Ty& zNear, const _Ty& zFar)
{
	
	const _Ty rad = fovy * _Ty(0.01745329251994329576923690768489);

	_Ty tanHalfFovy = Tan(rad / static_cast<_Ty>(2));

	Matrix4x4<_Ty> result;
	result._m00 = static_cast<_Ty>(1) / (aspect * tanHalfFovy);
	result._m11 = static_cast<_Ty>(1) / (tanHalfFovy);
	result._m22 = -(zFar + zNear) / (zFar - zNear);
	result._m23 = -static_cast<_Ty>(1);
	result._m32 = -(static_cast<_Ty>(2) * zFar * zNear) / (zFar - zNear);
	result._m33 = 0;
	return result;
}*/


}