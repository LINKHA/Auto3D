#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "BaseComponent.h"
#include "Math/Vector3.h"


AUTO_BEGIN
class Transform : public BaseComponent
{
public:
	Transform();
	~Transform();
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
};
AUTO_END
#endif // TRANSFORM_H_