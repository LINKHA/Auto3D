#ifndef TRANSFORM_H_
#define TRANSFORM_H_
#include "CBaseCompontent.h"
#include "Math/Vector3.h"


AUTO_BEGIN
class Transform : public CBaseCompontent
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