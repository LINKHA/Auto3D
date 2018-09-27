#include "DebugNew.h"

namespace Auto3D {

void DetectMemoryLeaks(bool enable)
{
	int flags = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	if (!enable)
	{
		flags &= ~_CRTDBG_LEAK_CHECK_DF;
	}
	else
	{
		flags |= _CRTDBG_LEAK_CHECK_DF;
		_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
		_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
	}
	_CrtSetDbgFlag(flags);
}

}