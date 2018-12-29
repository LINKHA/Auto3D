#include "IO.h"
#include "NewDef.h"
namespace Auto3D {

IO::IO(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	_monitors = MakeShared<Monitors>();
}


IO::~IO()
{
}

}