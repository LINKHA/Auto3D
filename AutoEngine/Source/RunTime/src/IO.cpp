#include "IO.h"
#include "DebugNew.h"
namespace Auto3D {

IO::IO(Ambient* ambient)
	:Super(ambient)
{
	_monitors = new Monitors();
}


IO::~IO()
{
	SAFE_DELETE(_monitors);
}

}