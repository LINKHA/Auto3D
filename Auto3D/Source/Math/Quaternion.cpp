#include "Container/String.h"
#include "Quaternion.h"

#include <cstdio>
#include <cstdlib>

#include "Debug/DebugNew.h"

namespace Auto3D
{

const FQuaternion FQuaternion::IDENTITY;

void FQuaternion::FromAngleAxis(float angle, const TVector3F& axis)
{
    TVector3F normAxis = axis.Normalized();
    angle *= M_DEGTORAD_2;
    float sinAngle = sinf(angle);
    float cosAngle = cosf(angle);
    
    _w = cosAngle;
    _x = normAxis._x * sinAngle;
    _y = normAxis._y * sinAngle;
    _z = normAxis._z * sinAngle;
}

void FQuaternion::FromEulerAngles(float x, float y, float z)
{
    // Order of rotations: Z first, then X, then Y (mimics typical FPS camera with gimbal lock at top/bottom)
    x *= M_DEGTORAD_2;
    y *= M_DEGTORAD_2;
    z *= M_DEGTORAD_2;
    float sinX = sinf(x);
    float cosX = cosf(x);
    float sinY = sinf(y);
    float cosY = cosf(y);
    float sinZ = sinf(z);
    float cosZ = cosf(z);
    
    _w = cosY * cosX * cosZ + sinY * sinX * sinZ;
    _x = cosY * sinX * cosZ + sinY * cosX * sinZ;
    _y = sinY * cosX * cosZ - cosY * sinX * sinZ;
    _z = cosY * cosX * sinZ - sinY * sinX * cosZ;
}

void FQuaternion::FromRotationTo(const TVector3F& start, const TVector3F& end)
{
    TVector3F normStart = start.Normalized();
    TVector3F normEnd = end.Normalized();
    float d = normStart.DotProduct(normEnd);
    
    if (d > -1.0f + M_EPSILON)
    {
        TVector3F c = normStart.CrossProduct(normEnd);
        float s = sqrtf((1.0f + d) * 2.0f);
        float invS = 1.0f / s;
        
        _x = c._x * invS;
        _y = c._y * invS;
        _z = c._z * invS;
        _w = 0.5f * s;
    }
    else
    {
        TVector3F axis = TVector3F::RIGHT.CrossProduct(normStart);
        if (axis.Length() < M_EPSILON)
            axis = TVector3F::UP.CrossProduct(normStart);
        
        FromAngleAxis(180.f, axis);
    }
}

void FQuaternion::FromAxes(const TVector3F& xAxis, const TVector3F& yAxis, const TVector3F& zAxis)
{
    TMatrix3x3F matrix(
        xAxis._x, yAxis._x, zAxis._x,
        xAxis._y, yAxis._y, zAxis._y,
        xAxis._z, yAxis._z, zAxis._z
    );
    
    FromRotationMatrix(matrix);
}

void FQuaternion::FromRotationMatrix(const TMatrix3x3F& matrix)
{
    float t = matrix._m00 + matrix._m11 + matrix._m22;
    
    if (t > 0.0f)
    {
        float invS = 0.5f / sqrtf(1.0f + t);
        
        _x = (matrix._m21 - matrix._m12) * invS;
        _y = (matrix._m02 - matrix._m20) * invS;
        _z = (matrix._m10 - matrix._m01) * invS;
        _w = 0.25f / invS;
    }
    else
    {
        if (matrix._m00 > matrix._m11 && matrix._m00 > matrix._m22)
        {
            float invS = 0.5f / sqrtf(1.0f + matrix._m00 - matrix._m11 - matrix._m22);
            
            _x = 0.25f / invS;
            _y = (matrix._m01 + matrix._m10) * invS;
            _z = (matrix._m20 + matrix._m02) * invS;
            _w = (matrix._m21 - matrix._m12) * invS;
        }
        else if (matrix._m11 > matrix._m22)
        {
            float invS = 0.5f / sqrtf(1.0f + matrix._m11 - matrix._m00 - matrix._m22);
            
            _x = (matrix._m01 + matrix._m10) * invS;
            _y = 0.25f / invS;
            _z = (matrix._m12 + matrix._m21) * invS;
            _w = (matrix._m02 - matrix._m20) * invS;
        }
        else
        {
            float invS = 0.5f / sqrtf(1.0f + matrix._m22 - matrix._m00 - matrix._m11);
            
            _x = (matrix._m02 + matrix._m20) * invS;
            _y = (matrix._m12 + matrix._m21) * invS;
            _z = 0.25f / invS;
            _w = (matrix._m10 - matrix._m01) * invS;
        }
    }
}

bool FQuaternion::FromLookRotation(const TVector3F& direction, const TVector3F& upDirection)
{
    FQuaternion ret;
    TVector3F forward = direction.Normalized();

    TVector3F v = forward.CrossProduct(upDirection);
    // If direction & upDirection are parallel and crossproduct becomes zero, use FromRotationTo() fallback
    if (v.LengthSquared() >= M_EPSILON)
    {
        v.Normalize();
        TVector3F up = v.CrossProduct(forward);
        TVector3F right = up.CrossProduct(forward);
        ret.FromAxes(right, up, forward);
    }
    else
        ret.FromRotationTo(TVector3F::FORWARD, forward);

    if (!ret.IsNaN())
    {
        (*this) = ret;
        return true;
    }
    else
        return false;
}


bool FQuaternion::FromString(const FString& str)
{
    return FromString(str.CString());
}

bool FQuaternion::FromString(const char* str)
{
    size_t elements = FString::CountElements(str, ' ');
    if (elements < 3)
        return false;
    
    char* ptr = (char*)str;
    if (elements < 4)
    {
        // 3 coords specified: conversion from Euler angles
        float x_, y_, z_;
        x_ = (float)strtod(ptr, &ptr);
        y_ = (float)strtod(ptr, &ptr);
        z_ = (float)strtod(ptr, &ptr);
        FromEulerAngles(x_, y_, z_);
    }
    else
    {
        // 4 coords specified: full quaternion
        _w = (float)strtod(ptr, &ptr);
        _x = (float)strtod(ptr, &ptr);
        _y = (float)strtod(ptr, &ptr);
        _z = (float)strtod(ptr, &ptr);
    }
    
    return true;
}

TVector3F FQuaternion::EulerAngles() const
{
    // Derivation from http://www.geometrictools.com/Documentation/EulerAngles.pdf
    // Order of rotations: Z first, then X, then Y
    float check = 2.0f * (-_y * _z + _w * _x);
    
    if (check < -0.995f)
    {
        return TVector3F(
            -90.0f,
            0.0f,
            -atan2f(2.0f * (_x * _z - _w * _y), 1.0f - 2.0f * (_y * _y + _z * _z)) * M_RADTODEG
        );
    }
    else if (check > 0.995f)
    {
        return TVector3F(
            90.0f,
            0.0f,
            atan2f(2.0f * (_x * _z - _w * _y), 1.0f - 2.0f * (_y * _y + _z * _z)) * M_RADTODEG
        );
    }
    else
    {
        return TVector3F(
            asinf(check) * M_RADTODEG,
            atan2f(2.0f * (_x * _z + _w * _y), 1.0f - 2.0f * (_x * _x + _y * _y)) * M_RADTODEG,
            atan2f(2.0f * (_x * _y + _w * _z), 1.0f - 2.0f * (_x * _x + _z * _z)) * M_RADTODEG
        );
    }
}

float FQuaternion::YawAngle() const
{
    return EulerAngles()._y;
}

float FQuaternion::PitchAngle() const
{
    return EulerAngles()._x;
}

float FQuaternion::RollAngle() const
{
    return EulerAngles()._z;
}

TMatrix3x3F FQuaternion::RotationMatrix() const
{
    return TMatrix3x3F(
        1.0f - 2.0f * _y * _y - 2.0f * _z * _z,
        2.0f * _x * _y - 2.0f * _w * _z,
        2.0f * _x * _z + 2.0f * _w * _y,
        2.0f * _x * _y + 2.0f * _w * _z,
        1.0f - 2.0f * _x * _x - 2.0f * _z * _z,
        2.0f * _y * _z - 2.0f * _w * _x,
        2.0f * _x * _z - 2.0f * _w * _y,
        2.0f * _y * _z + 2.0f * _w * _x,
        1.0f - 2.0f * _x * _x - 2.0f * _y * _y
    );
}

FQuaternion FQuaternion::Slerp(FQuaternion rhs, float t) const
{
    float cosAngle = DotProduct(rhs);
    // Enable shortest path rotation
    if (cosAngle < 0.0f)
    {
        cosAngle = -cosAngle;
        rhs = -rhs;
    }
    
    float angle = acosf(cosAngle);
    float sinAngle = sinf(angle);
    float t1, t2;
    
    if (sinAngle > 0.001f)
    {
        float invSinAngle = 1.0f / sinAngle;
        t1 = sinf((1.0f - t) * angle) * invSinAngle;
        t2 = sinf(t * angle) * invSinAngle;
    }
    else
    {
        t1 = 1.0f - t;
        t2 = t;
    }
    
    return *this * t1 + rhs * t2;
}

FQuaternion FQuaternion::Nlerp(FQuaternion rhs, float t, bool shortestPath) const
{
    FQuaternion result;
    float fCos = DotProduct(rhs);
    if (fCos < 0.0f && shortestPath)
        result = (*this) + (((-rhs) - (*this)) * t);
    else
        result = (*this) + ((rhs - (*this)) * t);
    result.Normalize();
    return result;
}

FString FQuaternion::ToString() const
{
    char tempBuffer[CONVERSION_BUFFER_LENGTH];
    sprintf(tempBuffer, "%g %g %g %g", _w, _x, _y, _z);
    return FString(tempBuffer);
}

}
