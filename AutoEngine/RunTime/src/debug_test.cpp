#include "debug_test.h"


AUTO_BEGIN
debug_test::debug_test(Ambient* ambient)
	:Super(ambient)
{
}


debug_test::~debug_test()
{
}
void debug_test::Start()
{
	//Print(This::GetSharedPtrTypeStringStatic());
}

AUTO_END