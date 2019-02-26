#pragma once

#include "Vector3.h"

namespace Auto3D
{

/// Four-dimensional vector.
class AUTO_API Vector4
{
public:
    /// X coordinate.
    float _x;
    /// Y coordinate.
    float _y;
    /// Z coordinate.
    float _z;
    /// W coordinate.
    float _w;
    
    /// Construct undefined.
    Vector4()
    {
    }
    
    /// Copy-construct.
    Vector4(const Vector4& vector) :
        _x(vector._x),
        _y(vector._y),
        _z(vector._z),
        _w(vector._w)
    {
    }
    
    /// Construct from a 3-dimensional vector and the W coordinate.
    Vector4(const Vector3& vector, float w) :
        _x(vector._x),
        _y(vector._y),
        _z(vector._z),
        _w(w)
    {
    }
    
    /// Construct from coordinates.
    Vector4(float x, float y, float z, float w) :
        _x(x),
        _y(y),
        _z(z),
        _w(w)
    {
    }
    
    /// Construct from a float array.
    Vector4(const float* data) :
        _x(data[0]),
        _y(data[1]),
        _z(data[2]),
        _w(data[3])
    {
    }
    
    /// Assign from another vector.
    Vector4& operator = (const Vector4& rhs)
    {
        _x = rhs._x;
        _y = rhs._y;
        _z = rhs._z;
        _w = rhs._w;
        return *this;
    }
   
    /// Construct by parsing a string.
    Vector4(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Vector4(const char* str)
    {
        FromString(str);
    }
    
    /// Test for equality with another vector without epsilon.
    bool operator == (const Vector4& rhs) const { return _x == rhs._x && _y == rhs._y && _z == rhs._z && _w == rhs._w; }
    /// Test for inequality with another vector without epsilon.
    bool operator != (const Vector4& rhs) const { return !(*this == rhs); }
    /// Add a vector.
    Vector4 operator + (const Vector4& rhs) const { return Vector4(_x + rhs._x, _y + rhs._y, _z + rhs._z, _w + rhs._w); }
    /// Return negation.
    Vector4 operator - () const { return Vector4(-_x, -_y, -_z, -_w); }
    /// Subtract a vector.
    Vector4 operator - (const Vector4& rhs) const { return Vector4(_x - rhs._x, _y - rhs._y, _z - rhs._z, _w - rhs._w); }
    /// Multiply with a scalar.
    Vector4 operator * (float rhs) const { return Vector4(_x * rhs, _y * rhs, _z * rhs, _w * rhs); }
    /// Multiply with a vector.
    Vector4 operator * (const Vector4& rhs) const { return Vector4(_x * rhs._x, _y * rhs._y, _z * rhs._z, _w * rhs._w); }
    /// Divide by a scalar.
    Vector4 operator / (float rhs) const { return Vector4(_x / rhs, _y / rhs, _z / rhs, _w / rhs); }
    /// Divide by a vector.
    Vector4 operator / (const Vector4& rhs) const { return Vector4(_x / rhs._x, _y / rhs._y, _z / rhs._z, _w / rhs._w); }
    
    /// Add-assign a vector.
    Vector4& operator += (const Vector4& rhs)
    {
        _x += rhs._x;
        _y += rhs._y;
        _z += rhs._z;
        _w += rhs._w;
        return *this;
    }
    
    /// Subtract-assign a vector.
    Vector4& operator -= (const Vector4& rhs)
    {
        _x -= rhs._x;
        _y -= rhs._y;
        _z -= rhs._z;
        _w -= rhs._w;
        return *this;
    }
    
    /// Multiply-assign a scalar.
    Vector4& operator *= (float rhs)
    {
        _x *= rhs;
        _y *= rhs;
        _z *= rhs;
        _w *= rhs;
        return *this;
    }
    
    /// Multiply-assign a vector.
    Vector4& operator *= (const Vector4& rhs)
    {
        _x *= rhs._x;
        _y *= rhs._y;
        _z *= rhs._z;
        _w *= rhs._w;
        return *this;
    }
    
    /// Divide-assign a scalar.
    Vector4& operator /= (float rhs)
    {
        float invRhs = 1.0f / rhs;
        _x *= invRhs;
        _y *= invRhs;
        _z *= invRhs;
        _w *= invRhs;
        return *this;
    }
    
    /// Divide-assign a vector.
    Vector4& operator /= (const Vector4& rhs)
    {
        _x /= rhs._x;
        _y /= rhs._y;
        _z /= rhs._z;
        _w /= rhs._w;
        return *this;
    }
    
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    
    /// Calculate dot product.
    float DotProduct(const Vector4& rhs) const { return _x * rhs._x + _y * rhs._y + _z * rhs._z + _w * rhs._w; }
    /// Calculate absolute dot product.
    float AbsDotProduct(const Vector4& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y) + Auto3D::Abs(_z * rhs._z) + Auto3D::Abs(_w * rhs._w); }
    /// Return absolute vector.
    Vector4 Abs() const { return Vector4(Auto3D::Abs(_x), Auto3D::Abs(_y), Auto3D::Abs(_z), Auto3D::Abs(_w)); }
    /// Linear interpolation with another vector.
    Vector4 Lerp(const Vector4& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }
    /// Test for equality with another vector with epsilon.
    bool Equals(const Vector4& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y) && Auto3D::Equals(_z, rhs._z) && Auto3D::Equals(_w, rhs._w); }
    /// Return whether is NaN.
    bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y) || Auto3D::IsNaN(_z) || Auto3D::IsNaN(_w); }
    
    /// Return float data.
    const float* Data() const { return &_x; }
    /// Return as string.
    String ToString() const;
    
    /// Zero vector.
    static const Vector4 ZERO;
    /// (1,1,1) vector.
    static const Vector4 ONE;
};

/// Multiply Vector4 with a scalar.
inline Vector4 operator * (float lhs, const Vector4& rhs) { return rhs * lhs; }

}
