#ifndef VECTOR_4_H
#define VECTOR_4_H
#include <math.h>


struct Vector4
{
public:
	Vector4()
		: x(0.0)
		, y(0.0)
		, z(0.0)
		, w(0.0)
	{}

	Vector4(const Vector4& vec)
		: x(vec.x)
		, y(vec.y)
		, z(vec.z)
		, w(vec.w)
	{}
	Vector4(float xPos, float yPos , float zPos, float wPos)
		:x(xPos)
		,y(yPos)
		,z(zPos)
		,w(wPos)
	{}

	Vector4&	Normalize();

	Vector4		operator-()const;
	Vector4		operator+(const Vector4& rhs)const;
	Vector4		operator*(const Vector4& rhs)const;
	Vector4		operator*(const float scale)const;
	Vector4     operator/(const float scale) const;			// inverse scale


	Vector4&	operator=(const Vector4& rhs);
	Vector4&    operator+=(const Vector4& rhs);				// add rhs and update this object
	Vector4&    operator-=(const Vector4& rhs);				// subtract rhs and update this object
	Vector4&    operator*=(const float scale);				// scale and update this object
	Vector4&    operator*=(const Vector4& rhs);				// multiply each element and update this object
	Vector4&    operator/=(const float scale);				// scale and update this object


	bool operator==(const Vector4& rhs)const;
	bool operator!=(const Vector4& rhs)const;
	bool operator<(const Vector4& rhs)const;



private:
	float x;
	float y;
	float z;
	float w;
};

#endif //VECTOR_4_H 