#pragma once
#include <Windows.h>
#define __OPENGL__
#ifdef __OPENGL__
#define AUTO_MAIN(function) \
int main() \
{ \
	return function;\
}
#endif

#ifdef __DIRECTX_11__
#define AUTO_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
	return function;\
}
#endif 

#ifdef __DIRECTX_12__
#define AUTO_MAIN(function) \
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd) \
{ \
	return function;\
}
#endif 