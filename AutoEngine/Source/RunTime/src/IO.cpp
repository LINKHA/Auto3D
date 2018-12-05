#include "IO.h"
#include "NewDef.h"
namespace Auto3D {

IO::IO(Ambient* ambient)
	:Super(ambient)
{
	_monitors = new Monitors();
}


IO::~IO()
{
	SafeDelete(_monitors);
}

}