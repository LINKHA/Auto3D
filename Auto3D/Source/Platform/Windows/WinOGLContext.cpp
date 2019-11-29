#include "AutoConfig.h"

#include "Container/WString.h"
#include "Debug/Log.h"
#include "WinOGLContext.h"
#include "Platform/Window.h"

#include <cstring>
#include <glad.h>

#include <Windows.h>
#include <wingdi.h>

#include "Debug/DebugNew.h"



namespace Auto3D
{

typedef BOOL(WINAPI* PFNWGLSWAPINTERVALEXTPROC) (int interval);
typedef BOOL(WINAPI* PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int* piAttribIList, const FLOAT* pfAttribFList, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGARBPROC) (HDC hdc);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC) (HDC hDC, HGLRC hShareContext, const int* attribList);

static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB = nullptr;
static PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
static PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT = nullptr;

#define WGL_DRAW_TO_WINDOW_ARB         0x2001
#define WGL_SUPPORT_OPENGL_ARB         0x2010
#define WGL_DOUBLE_BUFFER_ARB          0x2011
#define WGL_PIXEL_TYPE_ARB             0x2013
#define WGL_COLOR_BITS_ARB             0x2014
#define WGL_ALPHA_BITS_ARB             0x201B
#define WGL_DEPTH_BITS_ARB             0x2022
#define WGL_STENCIL_BITS_ARB           0x2023
#define WGL_TYPE_RGBA_ARB              0x202B
#define WGL_SAMPLE_BUFFERS_ARB         0x2041
#define WGL_SAMPLES_ARB                0x2042
#define WGL_CONTEXT_MAJOR_VERSION_ARB  0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB  0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB   0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001

GraphicsContext::GraphicsContext(Window* window) :
	_window(window),
	_contextHandle(nullptr)
{
}

GraphicsContext::~GraphicsContext()
{
	Release();
}

bool GraphicsContext::Create()
{
	if (_contextHandle)
		return true;

	if (!_window)
	{
		ErrorString("Window is null, can not create OpenGL context");
		return false;
	}

	// Create a dummy window & context for acquiring OpenGL 3.2 context creation functions
	// Assume that the window class name has already been registered by opening a Window
	HWND dummyWindowHandle = CreateWindowW(FWString(Window::className).CString(), L"", WS_POPUP | WS_DISABLED, CW_USEDEFAULT, CW_USEDEFAULT,
		10, 10, 0, 0, GetModuleHandle(0), nullptr);
	if (!dummyWindowHandle)
	{
		ErrorString("Failed to create a dummy window for OpenGL context creation");
		return false;
	}

	HDC dummyDCHandle = GetDC(dummyWindowHandle);
	if (!dummyDCHandle)
	{
		ErrorString("Failed to get DC for dummy window");
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof pfd);
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 0;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int pixelFormat = ChoosePixelFormat(dummyDCHandle, &pfd);
	if (!pixelFormat)
	{
		ErrorString("Failed to choose pixel format for dummy window");
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	DescribePixelFormat(dummyDCHandle, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (!SetPixelFormat(dummyDCHandle, pixelFormat, &pfd))
	{
		ErrorString("Failed to set pixel format for dummy window");
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	HGLRC dummyContextHandle = wglCreateContext(dummyDCHandle);
	if (!dummyContextHandle)
	{
		ErrorString("Failed to create OpenGL context for dummy window");
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	if (!wglMakeCurrent(dummyDCHandle, dummyContextHandle))
	{
		ErrorString("Failed to make OpenGL context current for dummy window");
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB");
	wglChoosePixelFormatARB = (PFNWGLCHOOSEPIXELFORMATARBPROC)wglGetProcAddress("wglChoosePixelFormatARB");
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");

	if (!wglChoosePixelFormatARB || !wglCreateContextAttribsARB)
	{
		ErrorString("WGL extension functions not available for creating an OpenGL context");
		wglMakeCurrent(0, 0);
		wglDeleteContext(dummyContextHandle);
		DestroyWindow(dummyWindowHandle);
		return false;
	}
	
	int multisample = _window->GetMultisample();
	// Now begin to create the real OpenGL context
	int pixelFormatAttrs[] =
	{
		WGL_SAMPLE_BUFFERS_ARB, multisample > 1 ? 1 : 0,
		WGL_SAMPLES_ARB, multisample > 1 ? multisample : 0,
		WGL_SUPPORT_OPENGL_ARB, true,
		WGL_PIXEL_TYPE_ARB, WGL_TYPE_RGBA_ARB,
		WGL_DRAW_TO_WINDOW_ARB, true,
		WGL_DOUBLE_BUFFER_ARB, true,
		WGL_COLOR_BITS_ARB, 32,
		WGL_DEPTH_BITS_ARB, 24,
		WGL_STENCIL_BITS_ARB, 8,
		0
	};

	unsigned numFormats = 0;
	wglChoosePixelFormatARB((HDC)GetDC((HWND)_window->Handle()), pixelFormatAttrs, nullptr, 1, &pixelFormat, &numFormats);
	/// \todo Handle fallback pixel formats
	if (!pixelFormat)
	{
		ErrorString("Failed to choose pixel format for OpenGL window");
		wglMakeCurrent(0, 0);
		wglDeleteContext(dummyContextHandle);
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	DescribePixelFormat((HDC)GetDC((HWND)_window->Handle()), pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);
	if (!SetPixelFormat((HDC)GetDC((HWND)_window->Handle()), pixelFormat, &pfd))
	{
		ErrorString("Failed to set pixel format for OpenGL window");
		wglMakeCurrent(0, 0);
		wglDeleteContext(dummyContextHandle);
		DestroyWindow(dummyWindowHandle);
		return false;
	}

	int contextAttrs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};

	_contextHandle = wglCreateContextAttribsARB((HDC)GetDC((HWND)_window->Handle()), 0, contextAttrs);
	wglMakeCurrent(0, 0);
	wglDeleteContext(dummyContextHandle);
	DestroyWindow(dummyWindowHandle);

	if (!_contextHandle)
	{
		ErrorString("Failed to create OpenGL context");
		return false;
	}

	// Initialize extensions that are needed during runtime now
	wglMakeCurrent((HDC)GetDC((HWND)_window->Handle()), (HGLRC)_contextHandle);
	wglSwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
	if (!gladLoadGL())
	{
		ErrorString("Failed to acquire OpenGL function pointers and extensions");
		Release();
		return false;
	}

	return true;
}

void GraphicsContext::SetVSync(bool enable)
{
	if (_contextHandle && wglSwapIntervalEXT)
		wglSwapIntervalEXT(enable ? 1 : 0);
}

void GraphicsContext::Present()
{
	if (_contextHandle)
		SwapBuffers((HDC)GetDC((HWND)_window->Handle()));

}

void GraphicsContext::Release()
{
	if (_contextHandle)
	{
		wglMakeCurrent(nullptr, nullptr);
		wglDeleteContext((HGLRC)_contextHandle);
	}
}

}

