#pragma once
#include "MathBase.h"
#include "Vector4.h"

namespace Auto3D {

/// RGBA color.
class Color
{
public:
	/// Construct with default values (opaque white.)
	Color() noexcept :
		r(1.0f),
		g(1.0f),
		b(1.0f),
		a(1.0f)
	{
	}

	/// Copy-construct from another color.
	Color(const Color& color) noexcept = default;

	/// Construct from another color and modify the alpha.
	Color(const Color& color, float a) noexcept :
		r(color.r),
		g(color.g),
		b(color.b),
		a(a)
	{
	}

	/// Construct from RGB values and set alpha fully opaque.
	Color(float r, float g, float b) noexcept :
		r(r),
		g(g),
		b(b),
		a(1.0f)
	{
	}

	/// Construct from RGBA values.
	Color(float r, float g, float b, float a) noexcept :
		r(r),
		g(g),
		b(b),
		a(a)
	{
	}

	/// Construct from a float array.
	explicit Color(const float* data) noexcept :
		r(data[0]),
		g(data[1]),
		b(data[2]),
		a(data[3])
	{
	}

	/// Assign from another color.
	Color& operator =(const Color& rhs) noexcept = default;

	/// Test for equality with another color without epsilon.
	bool operator ==(const Color& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

	/// Test for inequality with another color without epsilon.
	bool operator !=(const Color& rhs) const { return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a; }

	/// Multiply with a scalar.
	Color operator *(float rhs) const { return Color(r * rhs, g * rhs, b * rhs, a * rhs); }

	/// Add a color.
	Color operator +(const Color& rhs) const { return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); }

	/// Return negation.
	Color operator -() const { return Color(-r, -g, -b, -a); }

	/// Subtract a color.
	Color operator -(const Color& rhs) const { return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); }

	/// Add-assign a color.
	Color& operator +=(const Color& rhs)
	{
		r += rhs.r;
		g += rhs.g;
		b += rhs.b;
		a += rhs.a;
		return *this;
	}

	/// Return float data.
	const float* Data() const { return &r; }

	/// Return color packed to a 32-bit integer, with R component in the lowest 8 bits. Components are clamped to [0, 1] range.
	unsigned ToUInt() const;
	/// Return HSL color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
	Vector3 ToHSL() const;
	/// Return HSV color-space representation as a Vector3; the RGB values are clipped before conversion but not changed in the process.
	Vector3 ToHSV() const;
	/// Set RGBA values from packed 32-bit integer, with R component in the lowest 8 bits (format 0xAABBGGRR).
	void FromUInt(unsigned color);
	/// Set RGBA values from specified HSL values and alpha.
	void FromHSL(float h, float s, float l, float a = 1.0f);
	/// Set RGBA values from specified HSV values and alpha.
	void FromHSV(float h, float s, float v, float a = 1.0f);

	/// Return RGB as a three-dimensional vector.
	Vector3 ToVector3() const { return Vector3(r, g, b); }

	/// Return RGBA as a four-dimensional vector.
	Vector4 ToVector4() const { return Vector4(r, g, b, a); }

	/// Return sum of RGB components.
	float SumRGB() const { return r + g + b; }

	/// Return average value of the RGB channels.
	float Average() const { return (r + g + b) / 3.0f; }

	/// Return the 'grayscale' representation of RGB values, as used by JPEG and PAL/NTSC among others.
	float Luma() const { return r * 0.299f + g * 0.587f + b * 0.114f; }

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
	void Bounds(float* min, float* max, bool clipped = false) const;
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
	Color Abs() const { return Color(Auto3D::Abs(r), Auto3D::Abs(g), Auto3D::Abs(b), Auto3D::Abs(a)); }

	/// Test for equality with another color with epsilon.
	bool Equals(const Color& rhs) const
	{
		return Auto3D::Equals(r, rhs.r) && Auto3D::Equals(g, rhs.g) && Auto3D::Equals(b, rhs.b) && Auto3D::Equals(a, rhs.a);
	}

	/// Return as string.
	STRING ToString() const;

	/// Return hash value for HashSet & HashMap.
	unsigned ToHash() const { return ToUInt(); }

	/// Red value.
	float r;
	/// Green value.
	float g;
	/// Blue value.
	float b;
	/// Alpha value.
	float a;

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
	/// Transparent black color (black with no alpha).
	static const Color TRANSPARENT_BLACK;

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
inline Color operator *(float lhs, const Color& rhs) { return rhs * lhs; }


}


