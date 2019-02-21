#include "Vector4.h"

namespace Auto3D {

const Vector4 Vector4::ZERO;
const Vector4 Vector4::ONE(1.0f, 1.0f, 1.0f, 1.0f);

STRING Vector4::ToString() const
{
	char tempBuffer[KhSTL::CONVERSION_BUFFER_LENGTH];
	sprintf(tempBuffer, "%g %g %g %g", _x, _y, _z, _w);
	return STRING(tempBuffer);
}

}
