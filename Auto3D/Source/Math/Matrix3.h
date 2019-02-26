#pragma once

#include "Vector3.h"

namespace Auto3D
{

/// 3x3 matrix for rotation and scaling.
class AUTO_API Matrix3
{
public:
    /// Matrix values.
    float _m00, _m01, _m02;
    float _m10, _m11, _m12;
    float _m20, _m21, _m22;

    /// Construct undefined.
    Matrix3()
    {
    }
    
    /// Copy-construct.
    Matrix3(const Matrix3& matrix) :
        _m00(matrix._m00), _m01(matrix._m01), _m02(matrix._m02),
        _m10(matrix._m10), _m11(matrix._m11), _m12(matrix._m12),
        _m20(matrix._m20), _m21(matrix._m21), _m22(matrix._m22)
    {
    }
    
    /// Construct from values.
    Matrix3(float v00, float v01, float v02,
            float v10, float v11, float v12,
            float v20, float v21, float v22) :
        _m00(v00), _m01(v01), _m02(v02),
        _m10(v10), _m11(v11), _m12(v12),
        _m20(v20), _m21(v21), _m22(v22)
    {
    }
    
    /// Construct from a float array.
    Matrix3(const float* data) :
        _m00(data[0]), _m01(data[1]), _m02(data[2]),
        _m10(data[3]), _m11(data[4]), _m12(data[5]),
        _m20(data[6]), _m21(data[7]), _m22(data[8])
    {
    }
    
    /// Construct by parsing a string.
    Matrix3(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Matrix3(const char* str)
    {
        FromString(str);
    }
    
    /// Assign from another matrix.
    Matrix3& operator = (const Matrix3& rhs)
    {
        _m00 = rhs._m00; _m01 = rhs._m01; _m02 = rhs._m02;
        _m10 = rhs._m10; _m11 = rhs._m11; _m12 = rhs._m12;
        _m20 = rhs._m20; _m21 = rhs._m21; _m22 = rhs._m22;
        return *this;
    }
    
    /// Test for equality with another matrix without epsilon.
    bool operator == (const Matrix3& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();
        
        for (size_t i = 0; i < 9; ++i)
        {
            if (leftData[i] != rightData[i])
                return false;
        }
        
        return true;
    }
    
    /// Test for inequality with another matrix without epsilon.
    bool operator != (const Matrix3& rhs) const { return !(*this == rhs); }
    
    /// Multiply a Vector3.
    Vector3 operator * (const Vector3& rhs) const
    {
        return Vector3(
            _m00 * rhs._x + _m01 * rhs._y + _m02 * rhs._z,
            _m10 * rhs._x + _m11 * rhs._y + _m12 * rhs._z,
            _m20 * rhs._x + _m21 * rhs._y + _m22 * rhs._z
        );
    }
    
    /// Add a matrix.
    Matrix3 operator + (const Matrix3& rhs) const
    {
        return Matrix3(
            _m00 + rhs._m00, _m01 + rhs._m01, _m02 + rhs._m02,
            _m10 + rhs._m10, _m11 + rhs._m11, _m12 + rhs._m12,
            _m20 + rhs._m20, _m21 + rhs._m21, _m22 + rhs._m22
        );
    }
    
    /// Subtract a matrix.
    Matrix3 operator - (const Matrix3& rhs) const
    {
        return Matrix3(
            _m00 - rhs._m00, _m01 - rhs._m01, _m02 - rhs._m02,
            _m10 - rhs._m10, _m11 - rhs._m11, _m12 - rhs._m12,
            _m20 - rhs._m20, _m21 - rhs._m21, _m22 - rhs._m22
        );
    }
    
    /// Multiply with a scalar.
    Matrix3 operator * (float rhs) const
    {
        return Matrix3(
            _m00 * rhs, _m01 * rhs, _m02 * rhs,
            _m10 * rhs, _m11 * rhs, _m12 * rhs,
            _m20 * rhs, _m21 * rhs, _m22 * rhs
        );
    }
    
    /// Multiply a matrix.
    Matrix3 operator * (const Matrix3& rhs) const
    {
        return Matrix3(
            _m00 * rhs._m00 + _m01 * rhs._m10 + _m02 * rhs._m20,
            _m00 * rhs._m01 + _m01 * rhs._m11 + _m02 * rhs._m21,
            _m00 * rhs._m02 + _m01 * rhs._m12 + _m02 * rhs._m22,
            _m10 * rhs._m00 + _m11 * rhs._m10 + _m12 * rhs._m20,
            _m10 * rhs._m01 + _m11 * rhs._m11 + _m12 * rhs._m21,
            _m10 * rhs._m02 + _m11 * rhs._m12 + _m12 * rhs._m22,
            _m20 * rhs._m00 + _m21 * rhs._m10 + _m22 * rhs._m20,
            _m20 * rhs._m01 + _m21 * rhs._m11 + _m22 * rhs._m21,
            _m20 * rhs._m02 + _m21 * rhs._m12 + _m22 * rhs._m22
        );
    }
    
    /// Set scaling elements.
    void SetScale(const Vector3& scale)
    {
        _m00 = scale._x;
        _m11 = scale._y;
        _m22 = scale._z;
    }
    
    /// Set uniform scaling elements.
    void SetScale(float scale)
    {
        _m00 = scale;
        _m11 = scale;
        _m22 = scale;
    }
    
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);
    
    /// Return the scaling part.
    Vector3 Scale() const
    {
        return Vector3(
            sqrtf(_m00 * _m00 + _m10 * _m10 + _m20 * _m20),
            sqrtf(_m01 * _m01 + _m11 * _m11 + _m21 * _m21),
            sqrtf(_m02 * _m02 + _m12 * _m12 + _m22 * _m22)
        );
    }
    
    /// Return transpose.
    Matrix3 Transpose() const
    {
        return Matrix3(
            _m00, _m10, _m20,
            _m01, _m11, _m21,
            _m02, _m12, _m22
        );
    }
    
    /// Return scaled by a vector.
    Matrix3 Scaled(const Vector3& scale) const
    {
        return Matrix3(
            _m00 * scale._x, _m01 * scale._y, _m02 * scale._z,
            _m10 * scale._x, _m11 * scale._y, _m12 * scale._z,
            _m20 * scale._x, _m21 * scale._y, _m22 * scale._z
        );
    }
    
    /// Test for equality with another matrix with epsilon.
    bool Equals(const Matrix3& rhs) const
    {
        const float* leftData = Data();
        const float* rightData = rhs.Data();
        
        for (size_t i = 0; i < 9; ++i)
        {
            if (!Auto3D::Equals(leftData[i], rightData[i]))
                return false;
        }
        
        return true;
    }
    
    /// Return inverse.
    Matrix3 Inverse() const;
    
    /// Return float data.
    const float* Data() const { return &_m00; }
    /// Return as string.
    String ToString() const;
    
    /// Zero matrix.
    static const Matrix3 ZERO;
    /// Identity matrix.
    static const Matrix3 IDENTITY;
};

/// Multiply a 3x3 matrix with a scalar.
inline Matrix3 operator * (float lhs, const Matrix3& rhs) { return rhs * lhs; }

}
