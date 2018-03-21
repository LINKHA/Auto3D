#ifndef VECTOR_2_H_
#define VECTOR_2_H_
#include <math.h>
struct Vector2
{
public:
	Vector2()
		: x(0.0)
		, y(0.0)
	{}
	Vector2(const Vector2& vec)
	{
		x = vec.x;
		y = vec.y;
	}
	Vector2(float xPos, float yPos)
		: x(xPos)
		, y(yPos)
	{}

	static Vector2 Zero;

	float       Length() const;								// length of a vector
	float       Distance(const Vector2& vec) const;			// distance between two vectors
	float       Dot(const Vector2& vec) const;				// dot product
	Vector2&	 Normalize();								// normalized vector

	// operators
	Vector2     operator-()const;							// unary operator (negate)
	Vector2     operator+(const Vector2& rhs) const;		// add rhs
	Vector2     operator-(const Vector2& rhs) const;		// subtract rhs
	Vector2&    operator+=(const Vector2& rhs);				// add rhs and update this object
	Vector2&    operator-=(const Vector2& rhs);				// subtract rhs and update this object
	Vector2     operator*(const float scale) const;			// scale
	Vector2     operator*(const Vector2& rhs) const;		// multiply each element
	Vector2&    operator*=(const float scale);				// scale and update this object
	Vector2&    operator*=(const Vector2& rhs);				// multiply each element and update this object
	Vector2     operator/(const float scale) const;			// inverse scale
	Vector2&    operator/=(const float scale);				// scale and update this object
	Vector2&	operator=(const Vector2& rhs);


	bool        operator==(const Vector2& rhs) const;		// exact compare, no epsilon
	bool        operator!=(const Vector2& rhs) const;		// exact compare, no epsilon
	bool        operator<(const Vector2& rhs) const;		// comparison for sort

	float x;
	float y;
};


#endif // VECTOR_2_H_
