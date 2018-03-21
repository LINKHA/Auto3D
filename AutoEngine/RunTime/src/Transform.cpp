#include "Transform.h"

AUTO_BEGIN
Transform::Transform()
	: position(Vector3())
	, rotation(Quaternion())
	, scale (Vector3(1, 1, 1))
{
}


Transform::~Transform()
{
}
AUTO_END