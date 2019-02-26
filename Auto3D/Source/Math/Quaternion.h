#pragma once

#include "Matrix3x3.h"

namespace Auto3D
{

/// Rotation represented as a four-dimensional normalized vector.
class AUTO_API Quaternion
{
public:
    /// Construct undefined.
    Quaternion()
    {
    }
    
    /// Copy-construct.
    Quaternion(const Quaternion& quat) :
        _w(quat._w),
        _x(quat._x),
        _y(quat._y),
        _z(quat._z)
    {
    }
    
    /// Construct from values.
    Quaternion(float w, float x, float y, float z) :
        _w(w),
        _x(x),
        _y(y),
        _z(z)
    {
    }
    
    /// Construct from a float array.
    Quaternion(const float* data) :
        _w(data[0]),
        _x(data[1]),
        _y(data[2]),
        _z(data[3])
    {
    }
    
    /// Construct from an angle (in degrees) and axis.
    Quaternion(float angle, const Vector3F& axis)
    {
        FromAngleAxis(angle, axis);
    }

    /// Construct from a rotation angle (in degrees) about the Z axis.
    Quaternion(float angle)
    {
        FromAngleAxis(angle, Vector3F::FORWARD);
    }
    
    /// Construct from Euler angles (in degrees.)
    Quaternion(float x, float y, float z)
    {
        FromEulerAngles(x, y, z);
    }
    
    /// Construct from the rotation difference between two direction vectors.
    Quaternion(const Vector3F& start, const Vector3F& end)
    {
        FromRotationTo(start, end);
    }
    
    /// Construct from orthonormal axes.
    Quaternion(const Vector3F& xAxis, const Vector3F& yAxis, const Vector3F& zAxis)
    {
        FromAxes(xAxis, yAxis, zAxis);
    }

    /// Construct from a rotation matrix.
    Quaternion(const Matrix3x3F& matrix)
    {
        FromRotationMatrix(matrix);
    }
    
    /// Construct by parsing a string.
    Quaternion(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Quaternion(const char* str)
    {
        FromString(str);
    }
    
    /// Assign from another quaternion.
    Quaternion& operator = (const Quaternion& rhs)
    {
        _w = rhs._w;
        _x = rhs._x;
        _y = rhs._y;
        _z = rhs._z;
        return *this;
    }
    
    /// Add-assign a quaternion.
    Quaternion& operator += (const Quaternion& rhs)
    {
        _w += rhs._w;
        _x += rhs._x;
        _y += rhs._y;
        _z += rhs._z;
        return *this;
    }
  
    /// Multiply-assign a scalar.
    Quaternion& operator *= (float rhs)
    {
        _w *= rhs;
        _x *= rhs;
        _y *= rhs;
        _z *= rhs;
        return *this;
    }
    
    /// Test for equality with another quaternion without epsilon.
    bool operator == (const Quaternion& rhs) const { return _w == rhs._w && _x == rhs._x && _y == rhs._y && _z == rhs._z; }
    /// Test for inequality with another quaternion without epsilon.
    bool operator != (const Quaternion& rhs) const { return !(*this == rhs); }
    /// Multiply with a scalar.
    Quaternion operator * (float rhs) const { return Quaternion(_w * rhs, _x * rhs, _y * rhs, _z * rhs); }
    /// Return negation.
    Quaternion operator - () const { return Quaternion(-_w, -_x, -_y, -_z); }
    /// Add a quaternion.
    Quaternion operator + (const Quaternion& rhs) const { return Quaternion(_w + rhs._w, _x + rhs._x, _y + rhs._y, _z + rhs._z); }
    /// Subtract a quaternion.
    Quaternion operator - (const Quaternion& rhs) const { return Quaternion(_w - rhs._w, _x - rhs._x, _y - rhs._y, _z - rhs._z); }
    
    /// Multiply a quaternion.
    Quaternion operator * (const Quaternion& rhs) const
    {
        return Quaternion(
            _w * rhs._w - _x * rhs._x - _y * rhs._y - _z * rhs._z,
            _w * rhs._x + _x * rhs._w + _y * rhs._z - _z * rhs._y,
            _w * rhs._y + _y * rhs._w + _z * rhs._x - _x * rhs._z,
            _w * rhs._z + _z * rhs._w + _x * rhs._y - _y * rhs._x
        );
    }
    
    /// Multiply a Vector3.
    Vector3F operator * (const Vector3F& rhs) const
    {
        Vector3F qVec(_x, _y, _z);
        Vector3F cross1(qVec.CrossProduct(rhs));
        Vector3F cross2(qVec.CrossProduct(cross1));
        
        return rhs + 2.0f * (cross1 * _w + cross2);
    }
    
    /// Define from an angle (in degrees) and axis.
    void FromAngleAxis(float angle, const Vector3F& axis);
    /// Define from Euler angles (in degrees.)
    void FromEulerAngles(float x, float y, float z);
    /// Define from the rotation difference between two direction vectors.
    void FromRotationTo(const Vector3F& start, const Vector3F& end);
    /// Define from orthonormal axes.
    void FromAxes(const Vector3F& xAxis, const Vector3F& yAxis, const Vector3F& zAxis);
    /// Define from a rotation matrix.
    void FromRotationMatrix(const Matrix3x3F& matrix);
    /// Define from a direction to look in and an up direction. Return true on success, or false if would result in a NaN, in which case the current value remains.
    bool FromLookRotation(const Vector3F& direction, const Vector3F& up = Vector3F::UP);
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);

    /// Normalize to unit length.
    void Normalize()
    {
        float lenSquared = LengthSquared();
        if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            _w *= invLen;
            _x *= invLen;
            _y *= invLen;
            _z *= invLen;
        }
    }
    
    /// Return normalized to unit length.
    Quaternion Normalized() const
    {
        float lenSquared = LengthSquared();
        if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            return *this * invLen;
        }
        else
            return *this;
    }
    
    /// Return inverse.
    Quaternion Inverse() const
    {
        float lenSquared = LengthSquared();
        if (lenSquared == 1.0f)
            return Conjugate();
        else if (lenSquared >= M_EPSILON)
            return Conjugate() * (1.0f / lenSquared);
        else
            return IDENTITY;
    }
    
    /// Return squared length.
    float LengthSquared() const { return _w * _w + _x * _x + _y * _y + _z * _z; }
    /// Calculate dot product.
    float DotProduct(const Quaternion& rhs) const { return _w * rhs._w + _x * rhs._x + _y * rhs._y + _z * rhs._z; }
    /// Test for equality with another quaternion with epsilon.
    bool Equals(const Quaternion& rhs) const { return Auto3D::Equals(_w, rhs._w) && Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z); }
    /// Return whether is NaN.
    bool IsNaN() const { return Auto3D::IsNaN(_w) || Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z); }
    /// Return conjugate.
    Quaternion Conjugate() const { return Quaternion(_w, -_x, -_y, -_z); }
    
    /// Return Euler angles in degrees.
    Vector3F EulerAngles() const;
    /// Return yaw angle in degrees.
    float YawAngle() const;
    /// Return pitch angle in degrees.
    float PitchAngle() const;
    /// Return roll angle in degrees.
    float RollAngle() const;
    /// Return the rotation matrix that corresponds to this quaternion.
    Matrix3x3F RotationMatrix() const;
    /// Spherical interpolation with another quaternion.
    Quaternion Slerp(Quaternion rhs, float t) const;
    /// Normalized linear interpolation with another quaternion.
    Quaternion Nlerp(Quaternion rhs, float t, bool shortestPath = false) const;
    /// Return float data.
    const float* Data() const { return &_w; }
    /// Return as string.
    String ToString() const;
    
    /// W coordinate.
    float _w;
    /// X coordinate.
    float _x;
    /// Y coordinate.
    float _y;
    /// Z coordinate.
    float _z;
    
    /// Identity quaternion.
    static const Quaternion IDENTITY;
};

}
