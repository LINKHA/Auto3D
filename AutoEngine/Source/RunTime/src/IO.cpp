#include "IO.h"
#include "NewDef.h"
namespace Auto3D {

IO::IO(Ambient* ambient)
	:Super(ambient)
{
	_monitors = MakeShared<Monitors>();
}


IO::~IO()
{
}

}