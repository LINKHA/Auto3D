#include "Vector4.h"

namespace Auto3D {

const Vector4 Vector4::ZERO;
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

STRING Vector4::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", x, y, z, w);
	return STRING(tempBuffer);
}

}
