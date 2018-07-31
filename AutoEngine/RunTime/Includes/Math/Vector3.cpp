#include "Vector3.h"

namespace Auto3D {
const Vector3 Vector3::zero = Vector3(0, 0, 0);
const Vector3 Vector3::one = Vector3(1.0f, 1.0f, 1.0f);
const Vector3 Vector3::xAxis = Vector3(1, 0, 0);
const Vector3 Vector3::yAxis = Vector3(0, 1, 0);
const Vector3 Vector3::zAxis = Vector3(0, 0, 1);




static inline float ClampedMove(float lhs, float rhs, float clampedDelta)
{
	float delta = rhs - lhs;
	if (delta > 0.0F)
		return lhs + min(delta, clampedDelta);
	else
		return lhs - min(-delta, clampedDelta);
}


//Vector3 RotateTowards(const Vector3& lhs, const Vector3& rhs, float angleMove, float magnitudeMove)
//{
//	float lhsMag = Magnitude(lhs);
//	float rhsMag = Magnitude(rhs);
//
//	if (lhsMag > MATH_EPSILON && rhsMag > MATH_EPSILON)
//	{
//		Vector3 lhsNorm = lhs / lhsMag;
//		Vector3 rhsNorm = rhs / rhsMag;
//
//		float dot = Dot(lhsNorm, rhsNorm);
//		if (dot > 1.0F - MATH_EPSILON)
//		{
//			return MoveTowards(lhs, rhs, magnitudeMove);
//		}
//		// directions are almost opposite
//		else if (dot < -1.0F + MATH_EPSILON)
//		{
//			Vector3 axis = OrthoNormalVectorFast(lhsNorm);
//			Matrix3x3 m;
//			m.SetAxisAngle(axis, angleMove);
//			Vector3 rotated = m.MultiplyPoint3(lhsNorm);
//			rotated *= ClampedMove(lhsMag, rhsMag, magnitudeMove);
//			return rotated;
//		}
//		// normal case
//		else
//		{
//			float angle = acos(dot);
//			Vector3 axis = Normalize(Cross(lhsNorm, rhsNorm));
//			Matrix3x3 m;
//			m.SetAxisAngle(axis, min(angleMove, angle));
//			Vector3 rotated = m.MultiplyPoint3(lhsNorm);
//			rotated *= ClampedMove(lhsMag, rhsMag, magnitudeMove);
//			return rotated;
//		}
//	}
//	// at least one of the vectors is almost zero
//	else
//	{
//		return MoveTowards(lhs, rhs, magnitudeMove);
//	}
//}

Vector3 MoveTowards(const Vector3& lhs, const Vector3& rhs, float clampedDistance)
{
	Vector3 delta = rhs - lhs;
	float sqrDelta = Dot(delta, delta);
	float sqrClampedDistance = clampedDistance * clampedDistance;
	if (sqrDelta > sqrClampedDistance)
	{
		float deltaMag = sqrt(sqrDelta);
		if (deltaMag > MATH_EPSILON)
			return lhs + delta / deltaMag * clampedDistance;
		else
			return lhs;
	}
	else
		return rhs;
}

//Vector3 Slerp(const Vector3& lhs, const Vector3& rhs, float t) {
//
//	float lhsMag = Magnitude(lhs);
//	float rhsMag = Magnitude(rhs);
//
//	if (lhsMag < Vector3::epsilon || rhsMag < Vector3::epsilon)
//		return Lerp(lhs, rhs, t);
//
//	float lerpedMagnitude = Lerp(lhsMag, rhsMag, t);
//
//	float dot = Dot(lhs, rhs) / (lhsMag * rhsMag);
//	// direction is almost the same
//	if (dot > 1.0F - MATH_EPSILON)
//	{
//		return Lerp(lhs, rhs, t);
//	}
//	// directions are almost opposite
//	else if (dot < -1.0F + MATH_EPSILON)
//	{
//		Vector3 lhsNorm = lhs / lhsMag;
//		Vector3 axis = OrthoNormalVectorFast(lhsNorm);
//		Matrix3x3f m;
//		m.SetAxisAngle(axis, MATH_PI * t);
//		Vector3 slerped = m.MultiplyPoint3(lhsNorm);
//		slerped *= lerpedMagnitude;
//		return slerped;
//	}
//	// normal case
//	else
//	{
//		Vector3 axis = Cross(lhs, rhs);
//		Vector3 lhsNorm = lhs / lhsMag;
//		axis = Normalize(axis);
//		float angle = acos(dot) * t;
//
//		Matrix3x3 m;
//		m.SetAxisAngle(axis, angle);
//		Vector3 slerped = m.MultiplyPoint3(lhsNorm);
//		slerped *= lerpedMagnitude;
//		return slerped;
//	}
//}


}