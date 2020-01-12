#include "PlatformSupports/PlatformContext.h"
namespace Auto3D
{
	PlatfromContext platfromContext;

}

int main(int _argc, char** _argv)
{
	using namespace Auto3D;
	return platfromContext.run(_argc, _argv);
}

