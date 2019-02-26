#pragma once

#include "Vector4.h"

// Defined by Windows headers
#undef TRANSPARENT

namespace Auto3D
{

class String;

/// RGBA color.
class AUTO_API Color
{
public:
    /// Red value.
    float _r;
    /// Green value.
    float _g;
    /// Blue value.
    float _b;
    /// Alpha value.
    float _a;
	///// The alias of r
	//float& _red = _r;
	///// The alias of g
	//float& _green = _g;
	///// The alias of b
	//float& _blue = _b;
	///// The alias of a
	//float& _alpha = _a;


    /// Construct undefined.
    Color()
    {
    }
    
    /// Copy-construct.
    Color(const Color& color) :
        _r(color._r),
        _g(color._g),
        _b(color._b),
        _a(color._a)
    {
    }
    
    /// Construct from another color and modify the alpha.
    Color(const Color& color, float a) :
        _r(color._r),
        _g(color._g),
        _b(color._b),
        _a(a)
    {
    }
    
    /// Construct from RGB values and set alpha fully opaque.
    Color(float r, float g, float b) :
        _r(r),
        _g(g),
        _b(b),
        _a(1.0f)
    {
    }
    
    /// Construct from RGBA values.
    Color(float r, float g, float b, float a) :
        _r(r),
        _g(g),
        _b(b),
        _a(a)
    {
    }

    /// Construct from a float array.
    Color(const float* data) :
        _r(data[0]),
        _g(data[1]),
        _b(data[2]),
        _a(data[3])
    {
    }
    
    /// Construct by parsing a string.
    Color(const String& str)
    {
        FromString(str);
    }
    
    /// Construct by parsing a C string.
    Color(const char* str)
    {
        FromString(str);
    }
    
    /// Add-assign a color.
    Color& operator += (const Color& rhs)
    {
        _r += rhs._r;
        _g += rhs._g;
        _b += rhs._b;
        _a += rhs._a;
        return *this;
    }
    
    /// Test for equality with another color without epsilon.
    bool operator == (const Color& rhs) const { return _r == rhs._r && _g == rhs._g && _b == rhs._b && _a == rhs._a; }
    /// Test for inequality with another color without epsilon.
    bool operator != (const Color& rhs) const { return !(*this == rhs); }
    /// Multiply with a scalar.
    Color operator * (float rhs) const { return Color(_r * rhs, _g * rhs, _b * rhs, _a * rhs); }
    /// Add a color.
    Color operator + (const Color& rhs) const { return Color(_r + rhs._r, _g + rhs._g, _b + rhs._b, _a + rhs._a); }
    /// Substract a color.
    Color operator - (const Color& rhs) const { return Color(_r - rhs._r, _g - rhs._g, _b - rhs._b, _a - rhs._a); }
	///// Assign from another color.
	//Color& operator =(const Color& rhs) noexcept 
	//{
	//	/*_alpha = rhs._alpha;
	//	_red = rhs._red;
	//	_green = rhs._green;
	//	_blue = rhs._blue;*/
	//	_a = rhs._a;
	//	_r = rhs._r;
	//	_g = rhs._g;
	//	_b = rhs._b;
	//	return *this;
	//}

    /// Return float data.
    const float* Data() const { return &_r; }

    /// Return color packed to a 32-bit integer, with R component in the lowest 8 bits. Components are clamped to [0, 1] range.
    unsigned ToUInt() const;
    /// Return HSL color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
    Vector3F ToHSL() const;
    /// Return HSV color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
    Vector3F ToHSV() const;
    /// Set RGBA values from specified HSL values and alpha.
    void FromHSL(float h, float s, float l, float a = 1.0f);
    /// Set RGBA values from specified HSV values and alpha.
    void FromHSV(float h, float s, float v, float a = 1.0f);
    /// Parse from a string. Return true on success.
    bool FromString(const String& str);
    /// Parse from a C string. Return true on success.
    bool FromString(const char* str);

    /// Return RGB as a three-dimensional vector.
    Vector3F ToVector3() const { return Vector3F(_r, _g, _b); }
    /// Return RGBA as a four-dimensional vector.
    Vector4F ToVector4() const { return Vector4F(_r, _g, _b, _a); }

    /// Return sum of RGB components.
    float SumRGB() const { return _r + _g + _b; }
    /// Return average value of the RGB channels.
    float Average() const { return (_r + _g + _b) / 3.0f; }

    /// Return the 'grayscale' representation of RGB values, as used by JPEG and PAL/NTSC among others.
    float Luma() const { return _r * 0.299f + _g * 0.587f + _b * 0.114f; }
    /// Return the colorfulness relative to the brightness of a similarly illuminated white.
    float Chroma() const;
    /// Return hue mapped to range [0, 1.0).
    float Hue() const;
    /// Return saturation as defined for HSL.
    float SaturationHSL() const;
    /// Return saturation as defined for HSV.
    float SaturationHSV() const;
    /// Return value as defined for HSV: largest value of the RGB components. Equivalent to calling MinRGB().
    float Value() const { return MaxRGB(); }
    /// Return lightness as defined for HSL: average of the largest and smallest values of the RGB components.
    float Lightness() const;

    /// Stores the values of least and greatest RGB component at specified pointer addresses, optionally clipping those values to [0, 1] range.
    void Bounds(float* min, float* max, bool clipped=false) const;
    /// Return the largest value of the RGB components.
    float MaxRGB() const;
    /// Return the smallest value of the RGB components.
    float MinRGB() const;
    /// Return range, defined as the difference between the greatest and least RGB component.
    float Range() const;

    /// Clip to [0, 1.0] range.
    void Clip(bool clipAlpha = false);
    /// Inverts the RGB channels and optionally the alpha channel as well.
    void Invert(bool invertAlpha = false);
    /// Return linear interpolation of this color with another color.
    Color Lerp(const Color& rhs, float t) const;
    /// Return color with absolute components.
    Color Abs() const { return Color(Auto3D::Abs(_r), Auto3D::Abs(_g), Auto3D::Abs(_b), Auto3D::Abs(_a)); }
    /// Test for equality with another color with epsilon.
    bool Equals(const Color& rhs) const { return Auto3D::Equals(_r, rhs._r) && Auto3D::Equals(_g, rhs._g) && Auto3D::Equals(_b, rhs._b) && Auto3D::Equals(_a, rhs._a); }
    
    /// Return as string.
    String ToString() const;
    
    /// Opaque white color.
    static const Color WHITE;
    /// Opaque gray color.
    static const Color GRAY;
    /// Opaque black color.
    static const Color BLACK;
    /// Opaque red color.
    static const Color RED;
    /// Opaque green color.
    static const Color GREEN;
    /// Opaque blue color.
    static const Color BLUE;
    /// Opaque cyan color.
    static const Color CYAN;
    /// Opaque magenta color.
    static const Color MAGENTA;
    /// Opaque yellow color.
    static const Color YELLOW;
    /// Transparent color (black with no alpha).
    static const Color TRANSPARENT;

protected:
    /// Return hue value given greatest and least RGB component, value-wise.
    float Hue(float min, float max) const;
    /// Return saturation (HSV) given greatest and least RGB component, value-wise.
    float SaturationHSV(float min, float max) const;
    /// Return saturation (HSL) given greatest and least RGB component, value-wise.
    float SaturationHSL(float min, float max) const;
    /// Calculate and set RGB values. Convenience function used by FromHSV and FromHSL to avoid code duplication.
    void FromHCM(float h, float c, float m);
};

/// Multiply Color with a scalar.
inline Color operator * (float lhs, const Color& rhs) { return rhs * lhs; }

}
