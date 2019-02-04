#if defined(_WIN32)
#	include <windows.h>
#endif


#if defined(_WIN32)
	static HINSTANCE g_hInstance;
	static HINSTANCE g_prevInstance;
	static PSTR g_cmdLine;
	static int g_showCmd;
#endif
