#ifndef QUATERNION_H
#define QUATERNION_H

#include "AUMath.h"
#include "Auto.h"

MATH_BEGIN

struct Quaternion
{


};

#endif //!QUATERNION_H

MATH_END


//#ifndef QUATERNION_H
//#define QUATERNION_H
//
//#include "Matrix3x3.h"
//#include "Matrix4x4.h"
//#include "Vector3.h"
//#include "FloatConversion.h"
//#include <algorithm>
//#include <vector>
//#include "Runtime/Modules/ExportModules.h"
//
//class Quaternionf
//{
//public:
//
//	float x, y, z, w;
//
//	DEFINE_GET_TYPESTRING_IS_ANIMATION_CHANNEL(Quaternionf)
//		template<class TransferFunction> void Transfer(TransferFunction& transfer);
//
//	Quaternionf() {}
//	Quaternionf(float inX, float inY, float inZ, float inW);
//	explicit Quaternionf(const float* array) { x = array[0]; y = array[1]; z = array[2]; w = array[3]; }
//
//	// methods
//
//	const float* GetPtr()const { return &x; }
//	float* GetPtr() { return &x; }
//
//	const float& operator [] (int i)const { return GetPtr()[i]; }
//	float& operator [] (int i) { return GetPtr()[i]; }
//
//	void Set(float inX, float inY, float inZ, float inW);
//	void Set(const Quaternionf& aQuat);
//	void Set(const float* array) { x = array[0]; y = array[1]; z = array[2]; w = array[3]; }
//
//	friend Quaternionf Normalize(const Quaternionf& q) { return q / Magnitude(q); }
//	friend Quaternionf NormalizeSafe(const Quaternionf& q);
//
//	friend Quaternionf Conjugate(const Quaternionf& q);
//	friend Quaternionf Inverse(const Quaternionf& q);
//
//	friend float SqrMagnitude(const Quaternionf& q);
//	friend float Magnitude(const Quaternionf& q);
//
//	bool operator == (const Quaternionf& q)const { return x == q.x && y == q.y && z == q.z && w == q.w; }
//	bool operator != (const Quaternionf& q)const { return x != q.x || y != q.y || z != q.z || w != q.w; }
//
//	Quaternionf&	operator += (const Quaternionf&  aQuat);
//	Quaternionf&	operator -= (const Quaternionf&  aQuat);
//	Quaternionf&	operator *= (const float     	aScalar);
//	Quaternionf&	operator *= (const Quaternionf& 	aQuat);
//	Quaternionf&	operator /= (const float     	aScalar);
//
//	friend Quaternionf operator + (const Quaternionf& lhs, const Quaternionf& rhs)
//	{
//		Quaternionf q(lhs);
//		return q += rhs;
//	}
//
//	friend Quaternionf	operator - (const Quaternionf& lhs, const Quaternionf& rhs)
//	{
//		Quaternionf t(lhs);
//		return t -= rhs;
//	}
//
//	Quaternionf operator - () const
//	{
//		return Quaternionf(-x, -y, -z, -w);
//	}
//
//	Quaternionf	operator * (const float s) const
//	{
//		return Quaternionf(x*s, y*s, z*s, w*s);
//	}
//
//	friend Quaternionf	operator * (const float s, const Quaternionf& q)
//	{
//		Quaternionf t(q);
//		return t *= s;
//	}
//
//	friend Quaternionf	operator / (const Quaternionf& q, const float s)
//	{
//		Quaternionf t(q);
//		return t /= s;
//	}
//
//	inline friend Quaternionf operator * (const Quaternionf& lhs, const Quaternionf& rhs)
//	{
//		return Quaternionf(
//			lhs.w*rhs.x + lhs.x*rhs.w + lhs.y*rhs.z - lhs.z*rhs.y,
//			lhs.w*rhs.y + lhs.y*rhs.w + lhs.z*rhs.x - lhs.x*rhs.z,
//			lhs.w*rhs.z + lhs.z*rhs.w + lhs.x*rhs.y - lhs.y*rhs.x,
//			lhs.w*rhs.w - lhs.x*rhs.x - lhs.y*rhs.y - lhs.z*rhs.z);
//	}
//
//	static Quaternionf identity() { return Quaternionf(0.0F, 0.0F, 0.0F, 1.0F); }
//};
//
//bool CompareApproximately(const Quaternionf& q1, const Quaternionf& q2, float epsilon = Vector3f::epsilon);
//
//Quaternionf Lerp(const Quaternionf& q1, const Quaternionf& q2, float t);
//
//Quaternionf EXPORT_COREMODULE Slerp(const Quaternionf& q1, const Quaternionf& q2, float t);
//
//float Dot(const Quaternionf& q1, const Quaternionf& q2);
//
//Vector3f EXPORT_COREMODULE QuaternionToEuler(const Quaternionf& quat);
//
//std::vector<Vector3f> GetEquivalentEulerAngles(const Quaternionf& quat);
//
//Quaternionf EulerToQuaternion(const Vector3f& euler);
//
//void EXPORT_COREMODULE QuaternionToMatrix(const Quaternionf& q, Matrix3x3f& m);
//
//void EXPORT_COREMODULE MatrixToQuaternion(const Matrix3x3f& m, Quaternionf& q);
//void EXPORT_COREMODULE MatrixToQuaternion(const Matrix4x4f& m, Quaternionf& q);
//
//void QuaternionToMatrix(const Quaternionf& q, Matrix4x4f& m);
//
//void QuaternionToAxisAngle(const Quaternionf& q, Vector3f* axis, float* targetAngle);
//
//Quaternionf AxisAngleToQuaternion(const Vector3f& axis, float angle);
//
///// Generates a Right handed Quat from a look rotation. Returns if conversion was successful.
//bool LookRotationToQuaternion(const Vector3f& viewVec, const Vector3f& upVec, Quaternionf* res);
//
//
//inline Vector3f RotateVectorByQuat(const Quaternionf& lhs, const Vector3f& rhs)
//{
//	//	Matrix3x3f m;
//	//	QuaternionToMatrix (lhs, &m);
//	//	Vector3f restest = m.MultiplyVector3 (rhs);
//	float x = lhs.x * 2.0F;
//	float y = lhs.y * 2.0F;
//	float z = lhs.z * 2.0F;
//	float xx = lhs.x * x;
//	float yy = lhs.y * y;
//	float zz = lhs.z * z;
//	float xy = lhs.x * y;
//	float xz = lhs.x * z;
//	float yz = lhs.y * z;
//	float wx = lhs.w * x;
//	float wy = lhs.w * y;
//	float wz = lhs.w * z;
//
//	Vector3f res;
//	res.x = (1.0f - (yy + zz)) * rhs.x + (xy - wz)          * rhs.y + (xz + wy)          * rhs.z;
//	res.y = (xy + wz)          * rhs.x + (1.0f - (xx + zz)) * rhs.y + (yz - wx)          * rhs.z;
//	res.z = (xz - wy)          * rhs.x + (yz + wx)          * rhs.y + (1.0f - (xx + yy)) * rhs.z;
//
//	//	AssertIf (!CompareApproximately (restest, res));
//	return res;
//}
//
//// operator overloads
////  inlines
//
//inline Quaternionf::Quaternionf(float inX, float inY, float inZ, float inW)
//{
//	x = inX;
//	y = inY;
//	z = inZ;
//	w = inW;
//}
//
//template<class TransferFunction> inline
//void Quaternionf::Transfer(TransferFunction& transfer)
//{
//	transfer.AddMetaFlag(kTransferUsingFlowMappingStyle);
//	TRANSFER(x);
//	TRANSFER(y);
//	TRANSFER(z);
//	TRANSFER(w);
//}
//
//inline void Quaternionf::Set(float inX, float inY, float inZ, float inW)
//{
//	x = inX;
//	y = inY;
//	z = inZ;
//	w = inW;
//}
//
//inline void Quaternionf::Set(const Quaternionf& aQuat)
//{
//	x = aQuat.x;
//	y = aQuat.y;
//	z = aQuat.z;
//	w = aQuat.w;
//}
//
//inline Quaternionf Conjugate(const Quaternionf& q)
//{
//	return Quaternionf(-q.x, -q.y, -q.z, q.w);
//}
//
//inline Quaternionf Inverse(const Quaternionf& q)
//{
//	// Is it necessary to divide by SqrMagnitude???
//	Quaternionf res = Conjugate(q);
//	return res;
//}
//
//inline float Magnitude(const Quaternionf& q)
//{
//	return SqrtImpl(SqrMagnitude(q));
//}
//
//inline float SqrMagnitude(const Quaternionf& q)
//{
//	return Dot(q, q);
//}
//
//inline Quaternionf& Quaternionf::operator+= (const Quaternionf& aQuat)
//{
//	x += aQuat.x;
//	y += aQuat.y;
//	z += aQuat.z;
//	w += aQuat.w;
//	return *this;
//}
//
//inline Quaternionf& Quaternionf::operator-= (const Quaternionf& aQuat)
//{
//	x -= aQuat.x;
//	y -= aQuat.y;
//	z -= aQuat.z;
//	w -= aQuat.w;
//	return *this;
//}
//
//inline Quaternionf& Quaternionf::operator *= (float aScalar)
//{
//	x *= aScalar;
//	y *= aScalar;
//	z *= aScalar;
//	w *= aScalar;
//	return *this;
//}
//
//inline Quaternionf&	Quaternionf::operator /= (const float     	aScalar)
//{
//	AssertIf(CompareApproximately(aScalar, 0.0F));
//	x /= aScalar;
//	y /= aScalar;
//	z /= aScalar;
//	w /= aScalar;
//	return *this;
//}
//
//inline Quaternionf&	Quaternionf::operator *= (const Quaternionf& 	rhs)
//{
//	float tempx = w*rhs.x + x*rhs.w + y*rhs.z - z*rhs.y;
//	float tempy = w*rhs.y + y*rhs.w + z*rhs.x - x*rhs.z;
//	float tempz = w*rhs.z + z*rhs.w + x*rhs.y - y*rhs.x;
//	float tempw = w*rhs.w - x*rhs.x - y*rhs.y - z*rhs.z;
//	x = tempx; y = tempy; z = tempz; w = tempw;
//	return *this;
//}
//
//inline Quaternionf Lerp(const Quaternionf& q1, const Quaternionf& q2, float t)
//{
//	Quaternionf tmpQuat;
//	// if (dot < 0), q1 and q2 are more than 360 deg apart.
//	// The problem is that quaternions are 720deg of freedom.
//	// so we - all components when lerping
//	if (Dot(q1, q2) < 0.0F)
//	{
//		tmpQuat.Set(q1.x + t * (-q2.x - q1.x),
//			q1.y + t * (-q2.y - q1.y),
//			q1.z + t * (-q2.z - q1.z),
//			q1.w + t * (-q2.w - q1.w));
//	}
//	else
//	{
//		tmpQuat.Set(q1.x + t * (q2.x - q1.x),
//			q1.y + t * (q2.y - q1.y),
//			q1.z + t * (q2.z - q1.z),
//			q1.w + t * (q2.w - q1.w));
//	}
//	return Normalize(tmpQuat);
//}
//
//inline float Dot(const Quaternionf& q1, const Quaternionf& q2)
//{
//	return (q1.x*q2.x + q1.y*q2.y + q1.z*q2.z + q1.w*q2.w);
//}
//
//float AngularDistance(const Quaternionf& lhs, const Quaternionf& rhs);
//
//
//inline void QuaternionToAxisAngle(const Quaternionf& q, Vector3f* axis, float* targetAngle)
//{
//	AssertIf(!CompareApproximately(SqrMagnitude(q), 1.0F));
//	*targetAngle = 2.0f* acos(q.w);
//	if (CompareApproximately(*targetAngle, 0.0F))
//	{
//		*axis = Vector3f::xAxis;
//		return;
//	}
//
//	float div = 1.0f / sqrt(1.0f - Sqr(q.w));
//	axis->Set(q.x*div, q.y*div, q.z*div);
//}
//
//inline Quaternionf AxisAngleToQuaternion(const Vector3f& axis, float angle)
//{
//	Quaternionf q;
//	AssertIf(!CompareApproximately(SqrMagnitude(axis), 1.0F));
//	float halfAngle = angle * 0.5F;
//	float s = sin(halfAngle);
//
//	q.w = cos(halfAngle);
//	q.x = s * axis.x;
//	q.y = s * axis.y;
//	q.z = s * axis.z;
//	return q;
//}
//
//inline Quaternionf AngularVelocityToQuaternion(const Vector3f& axis, float deltaTime)
//{
//	float w = Magnitude(axis);
//	if (w > Vector3f::epsilon)
//	{
//		float v = deltaTime * w * 0.5f;
//		float q = cos(v);
//		float s = sin(v) / w;
//
//		Quaternionf integrated;
//		integrated.w = q;
//		integrated.x = s * axis.x;
//		integrated.y = s * axis.y;
//		integrated.z = s * axis.z;
//
//		return NormalizeSafe(integrated);
//	}
//	else
//	{
//		return Quaternionf::identity();
//	}
//}
//
//inline Quaternionf AxisAngleToQuaternionSafe(const Vector3f& axis, float angle)
//{
//	Quaternionf q;
//	float mag = Magnitude(axis);
//	if (mag > 0.000001F)
//	{
//		float halfAngle = angle * 0.5F;
//
//		q.w = cos(halfAngle);
//
//		float s = sin(halfAngle) / mag;
//		q.x = s * axis.x;
//		q.y = s * axis.y;
//		q.z = s * axis.z;
//		return q;
//	}
//	else
//	{
//		return Quaternionf::identity();
//	}
//}
//
//// Generates a quaternion that rotates lhs into rhs.
//Quaternionf FromToQuaternionSafe(const Vector3f& lhs, const Vector3f& rhs);
//// from and to are assumed to be normalized
//Quaternionf FromToQuaternion(const Vector3f& from, const Vector3f& to);
//
//
//inline bool CompareApproximately(const Quaternionf& q1, const Quaternionf& q2, float epsilon)
//{
//	//return SqrMagnitude (q1 - q2) < epsilon * epsilon;
//	return (SqrMagnitude(q1 - q2) < epsilon * epsilon) || (SqrMagnitude(q1 + q2) < epsilon * epsilon);
//	//return Abs (Dot (q1, q2)) > (1 - epsilon * epsilon);
//}
//
//inline Quaternionf NormalizeSafe(const Quaternionf& q)
//{
//	float mag = Magnitude(q);
//	if (mag < Vector3f::epsilon)
//		return Quaternionf::identity();
//	else
//		return q / mag;
//}
//
//inline Quaternionf NormalizeFastEpsilonZero(const Quaternionf& q)
//{
//	float m = SqrMagnitude(q);
//	if (m < Vector3f::epsilon)
//		return Quaternionf(0.0F, 0.0F, 0.0F, 0.0F);
//	else
//		return q * FastInvSqrt(m);
//}
//
//
//inline bool IsFinite(const Quaternionf& f)
//{
//	return IsFinite(f.x) & IsFinite(f.y) & IsFinite(f.z) & IsFinite(f.w);
//}
//
//
//#endif
