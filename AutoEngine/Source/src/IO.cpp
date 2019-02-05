#include "IO.h"
#include "NewDef.h"
namespace Auto3D {

IO::IO(SharedPtr<Ambient> ambient)
	:Super(ambient)
{
	_monitors = MakeShared<Monitors>();

	_engineInfo = MakeShared<EngineInfo>();

}


IO::~IO()
{
}

}