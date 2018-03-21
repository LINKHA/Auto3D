#ifndef VECTOR_3_H_
#define VECTOR_3_H_
#include <math.h>
struct Vector3
{
public:
	Vector3()
		: x(0.0)
		, y(0.0)
		, z(0.0)
	{}
	Vector3(const Vector3& vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
	}
	Vector3(float xPos, float yPos, float zPos)
		: x(xPos)
		, y(yPos)
		, z(zPos)
	{}

	static Vector3 Zero;
	static Vector3 Up;
	static Vector3 Down;
	static Vector3 Right;
	static Vector3 Left;

	float       Length() const;								// length of a vector
	float       Distance(const Vector3& vec) const;			// distance between two vectors
	float       Dot(const Vector3& vec) const;				// dot product
	Vector3&	 Normalize();								// normalized vector

															// operators
	
	Vector3     operator-()const;							// unary operator (negate)
	Vector3     operator+(const Vector3& rhs) const;		// add rhs
	Vector3     operator-(const Vector3& rhs) const;		// subtract rhs
	Vector3     operator/(const float scale) const;			// inverse scale
	Vector3     operator*(const float scale) const;			// scale
	Vector3     operator*(const Vector3& rhs) const;		// multiply each element


	Vector3&	operator=(const Vector3& rhs);
	Vector3&    operator+=(const Vector3& rhs);				// add rhs and update this object
	Vector3&    operator-=(const Vector3& rhs);				// subtract rhs and update this object	
	Vector3&    operator*=(const float scale);				// scale and update this object
	Vector3&    operator*=(const Vector3& rhs);				// multiply each element and update this object
	
	
	
	
	
	Vector3&    operator/=(const float scale);				// scale and update this object

	bool        operator==(const Vector3& rhs) const;		// exact compare, no epsilon
	bool        operator!=(const Vector3& rhs) const;		// exact compare, no epsilon
	bool        operator<(const Vector3& rhs) const;		// comparison for sort

	float x;
	float y;
	float z;
};


#endif // VECTOR_3_H_
