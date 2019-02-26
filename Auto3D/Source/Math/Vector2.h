#pragma once

#include "Math.h"
#include "../AutoConfig.h"

namespace Auto3D
{

class String;

/// Two-dimensional vector.
class AUTO_API Vector2
{
public:
    /// X coordinate.
    float _x;
    /// Y coordinate.
    float _y;
    
    /// Construct undefined.
    Vector2()
    {
    }
    
    /// Copy-construct.
    Vector2(const Vector2& vector) :
        _x(vector._x),
        _y(vector._y)
    {
    }
    
    /// Construct from coordinates.
    Vector2(float x, float y) :
        _x(x),
        _y(y)
    {
    }
    
    /// Construct from a float array.
    Vector2(const float* data) :
        _x(data[0]),
        _y(data[1])
    {
    }
    
    /// Construct by parsing a string.
    Vector2(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Vector2(const char* str)
    {
        FromString(str);
    }
    
    /// Assign from another vector.
    Vector2& operator = (const Vector2& rhs)
    {
        _x = rhs._x;
        _y = rhs._y;
        return *this;
    }
    
    /// Add-assign a vector.
    Vector2& operator += (const Vector2& rhs)
    {
        _x += rhs._x;
        _y += rhs._y;
        return *this;
    }
    
    /// Subtract-assign a vector.
    Vector2& operator -= (const Vector2& rhs)
    {
        _x -= rhs._x;
        _y -= rhs._y;
        return *this;
    }
    
    /// Multiply-assign a scalar.
    Vector2& operator *= (float rhs)
    {
        _x *= rhs;
        _y *= rhs;
        return *this;
    }
    
    /// Multiply-assign a vector.
    Vector2& operator *= (const Vector2& rhs)
    {
        _x *= rhs._x;
        _y *= rhs._y;
        return *this;
    }
    
    /// Divide-assign a scalar.
    Vector2& operator /= (float rhs)
    {
        float invRhs = 1.0f / rhs;
        _x *= invRhs;
        _y *= invRhs;
        return *this;
    }
    
    /// Divide-assign a vector.
    Vector2& operator /= (const Vector2& rhs)
    {
        _x /= rhs._x;
        _y /= rhs._y;
        return *this;
    }
    
    /// Normalize to unit length.
    void Normalize()
    {
        float lenSquared = LengthSquared();
        if (!Auto3D::Equals(lenSquared, 1.0f) && lenSquared > 0.0f)
        {
            float invLen = 1.0f / sqrtf(lenSquared);
            _x *= invLen;
            _y *= invLen;
        }
    }
    
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    
    /// Test for equality with another vector without epsilon.
    bool operator == (const Vector2& rhs) const { return _x == rhs._x && _y == rhs._y; }
    /// Test for inequality with another vector without epsilon.
    bool operator != (const Vector2& rhs) const { return !(*this == rhs); }
    /// Add a vector.
    Vector2 operator + (const Vector2& rhs) const { return Vector2(_x + rhs._x, _y + rhs._y); }
    /// Return negation.
    Vector2 operator - () const { return Vector2(-_x, -_y); }
    /// Subtract a vector.
    Vector2 operator - (const Vector2& rhs) const { return Vector2(_x - rhs._x, _y - rhs._y); }
    /// Multiply with a scalar.
    Vector2 operator * (float rhs) const { return Vector2(_x * rhs, _y * rhs); }
    /// Multiply with a vector.
    Vector2 operator * (const Vector2& rhs) const { return Vector2(_x * rhs._x, _y * rhs._y); }
    /// Divide by a scalar.
    Vector2 operator / (float rhs) const { return Vector2(_x / rhs, _y / rhs); }
    /// Divide by a vector.
    Vector2 operator / (const Vector2& rhs) const { return Vector2(_x / rhs._x, _y / rhs._y); }
    
    /// Return length.
    float Length() const { return sqrtf(_x * _x + _y * _y); }
    /// Return squared length.
    float LengthSquared() const { return _x * _x + _y * _y; }
    /// Calculate dot product.
    float DotProduct(const Vector2& rhs) const { return _x * rhs._x + _y * rhs._y; }
    /// Calculate absolute dot product.
    float AbsDotProduct(const Vector2& rhs) const { return Auto3D::Abs(_x * rhs._x) + Auto3D::Abs(_y * rhs._y); }
    /// Return absolute vector.
    Vector2 Abs() const { return Vector2(Auto3D::Abs(_x), Auto3D::Abs(_y)); }
    /// Linear interpolation with another vector.
    Vector2 Lerp(const Vector2& rhs, float t) const { return *this * (1.0f - t) + rhs * t; }
    /// Test for equality with another vector with epsilon.
    bool Equals(const Vector2& rhs) const { return Auto3D::Equals(_x, rhs._x) && Auto3D::Equals(_y, rhs._y); }
    /// Return whether is NaN.
    bool IsNaN() const { return Auto3D::IsNaN(_x) || Auto3D::IsNaN(_y); }
    
    /// Return normalized to unit length.
    Vector2 Normalized() const
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
    
    /// Return float data.
    const float* Data() const { return &_x; }
    /// Return as string.
    String ToString() const;
    
    /// Zero vector.
    static const Vector2 ZERO;
    /// (-1,0) vector.
    static const Vector2 LEFT;
    /// (1,0) vector.
    static const Vector2 RIGHT;
    /// (0,1) vector.
    static const Vector2 UP;
    /// (0,-1) vector.
    static const Vector2 DOWN;
    /// (1,1) vector.
    static const Vector2 ONE;
};

/// Multiply Vector2 with a scalar
inline Vector2 operator * (float lhs, const Vector2& rhs) { return rhs * lhs; }

}
