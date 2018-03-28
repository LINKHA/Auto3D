#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "BaseComponent.h"
#include "Math/AUMath.h"

USING_MATH
AUTO_BEGIN

class Transform : public BaseComponent
{
public:
	Transform();
	~Transform();

	Vector3 position;
	MATH::Quaternion rotation;
	Vector3 scale;
};
AUTO_END
#endif // TRANSFORM_H_