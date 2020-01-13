#include "Application.h"
#include "PlatformSupports/PlatformContext.h"

namespace Auto3D
{
//PlatfromContext platfromContext;
FApplication app;

FApplication::FApplication()
{

}

FApplication::~FApplication()
{

}

void FApplication::Run()
{
	int i = 0;
}

void FApplication::ErrorExit(const FString& message)
{

}
}

int main(int _argc, char** _argv)
{
	using namespace Auto3D;
	app.Run();
	return PlatfromContext::Get().run(_argc, _argv);
}

