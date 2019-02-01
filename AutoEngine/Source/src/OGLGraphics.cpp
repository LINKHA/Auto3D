#include "Graphics.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "NewDef.h"

namespace Auto3D {


static const unsigned glFillType[] =
{
	GL_FILL,
	GL_LINE,
	GL_POINT
};

static const unsigned glChangeMode[] =
{
	GL_STATIC_DRAW,
	GL_DYNAMIC_DRAW,
	GL_STREAM_DRAW
};

static const unsigned glBufferMode[] =
{
	GL_ARRAY_BUFFER ,
	GL_ELEMENT_ARRAY_BUFFER
};

static const unsigned glDepthMode[] =
{
	GL_ALWAYS,
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL
};

static const unsigned glStencilOps[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_INCR_WRAP,
	GL_DECR,
	GL_DECR_WRAP,
	GL_INVERT
};

static const unsigned glBlendSrcFu[] =
{
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA
};

static const unsigned glBlendDestFu[] =
{
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA
};

static const unsigned glBlendOp[] =
{
	GL_FUNC_ADD,
	GL_FUNC_REVERSE_SUBTRACT
};

static const unsigned glElementTypes[] =
{
	GL_INT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE
};

void WINAPI glDebugOutput(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return; // ignore these non-significant error codes

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case 0x8246:				std::cout << "Source: API"; break;
	case 0x8247:				std::cout << "Source: Window System"; break;
	case 0x8248:				std::cout << "Source: Shader Compiler"; break;
	case 0x8249:				std::cout << "Source: Third Party"; break;
	case 0x824A:				std::cout << "Source: Application"; break;
	case 0x824B:				std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case 0x824C:				std::cout << "Type: Error"; break;
	case 0x824D:				std::cout << "Type: Deprecated Behaviour"; break;
	case 0x824E:				std::cout << "Type: Undefined Behaviour"; break;
	case 0x824F:				std::cout << "Type: Portability"; break;
	case 0x8250:				std::cout << "Type: Performance"; break;
	case 0x8268:				std::cout << "Type: Marker"; break;
	case 0x8269:				std::cout << "Type: Push Group"; break;
	case 0x826A:				std::cout << "Type: Pop Group"; break;
	case 0x8251:				std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case 0x9146:				std::cout << "Severity: high"; break;
	case 0x9147:				std::cout << "Severity: medium"; break;
	case 0x9148:				std::cout << "Severity: low"; break;
	case 0x826B:				std::cout << "Severity: notification"; break;
	}
	std::cout << std::endl;
	std::cout << std::endl;
}

static void GetGLPrimitiveType(unsigned elementCount, PrimitiveTypes type, unsigned& primitiveCount, GLenum& glPrimitiveType)
{
	switch (type)
	{
	case PrimitiveTypes::kTringleList:
		primitiveCount = elementCount / 3;
		glPrimitiveType = GL_TRIANGLES;
		break;

	case PrimitiveTypes::kLineList:
		primitiveCount = elementCount / 2;
		glPrimitiveType = GL_LINES;
		break;

	case PrimitiveTypes::kPointList:
		primitiveCount = elementCount;
		glPrimitiveType = GL_POINTS;
		break;

	case PrimitiveTypes::kTriangleStrip:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_STRIP;
		break;

	case PrimitiveTypes::kLineStrip:
		primitiveCount = elementCount - 1;
		glPrimitiveType = GL_LINE_STRIP;
		break;

	case PrimitiveTypes::kTiangleFan:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_FAN;
		break;
	}
}

void Graphics::Init()
{
	_icon = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");

	stbi_set_flip_vertically_on_load(true);
	// Create game window
	CreateGameWindow();
	// Create device(OpenGL context and DirectX device)
	CreateDevice();
	// Load graphics API with SDL address
	LoadAPILoader();
	// Register graphics API debug
	RegisterDebug();
	// Create Icon
	CreateIcon();
}

void Graphics::RegisterDebug()
{
#ifdef _DEBUG
	GLint flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
}

bool Graphics::BeginFrame()
{
	if (!IsInitialized() || IsDeviceLost())
		return false;
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SetColorWrite(true);
	SetDepthWrite(true);
	Clear(CLEAR_COLOR | CLEAR_DEPTH | CLEAR_STENCIL);
	return true;
}
void Graphics::EndFrame()
{
	if (!IsInitialized())
		return;
	SDL_GL_SwapWindow(_window);
}

void Graphics::Clear(unsigned flags, const Color & color, float depth, unsigned stencil)
{
	bool oldColorWrite = _colorWrite;
	bool oldDepthWrite = _depthWrite;

	if (flags & CLEAR_COLOR && !oldColorWrite)
		SetColorWrite(true);
	if (flags & CLEAR_DEPTH && !oldDepthWrite)
		SetDepthWrite(true);
	if (flags & CLEAR_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(MATH_MAX_UNSIGNED);

	unsigned glFlags = 0;
	if (flags & CLEAR_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	if (flags & CLEAR_DEPTH)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & CLEAR_STENCIL)
	{
		glFlags |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(glFlags);
}


void Graphics::SetColorWrite(bool enable)
{
	if (enable != _colorWrite)
	{
		if (enable)
			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		else
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		_colorWrite = enable;
	}
}
void Graphics::SetDepthWrite(bool enable)
{
	if (enable != _depthWrite)
	{
		glDepthMask(enable ? GL_TRUE : GL_FALSE);
		_depthWrite = enable;
	}
}

void Graphics::CreateGameWindow()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
		ErrorString("Couldn't initialize SDL");
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

#if _OPENGL_4_6_
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
#endif // _OPENGL_4_6_

#if  _OPENGL_4_PLUS_ 
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif //  _OPENGL_4_PLUS_

#if _OPENGL_3_PLUS_ 
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif // _OPENGL_3_PLUS_

	// Also request a depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	// MSAA sample point
	CreateSamplePoint(_numSample);

	// Create the window
	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	if (_isFullScreen)
		flags |= SDL_WINDOW_FULLSCREEN;
	if (_isBorderless)
		flags |= SDL_WINDOW_BORDERLESS;
	if (_isResizable)
		flags |= SDL_WINDOW_RESIZABLE;
	if (_isHighDPI)
		flags |= SDL_WINDOW_ALLOW_HIGHDPI;

	// The position size will be reset later
	_window = SDL_CreateWindow(
		_titleName,
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		0, 0, flags
	);

	if (_window == NULL)
		ErrorString("Couldn't set video mode");

	// Init window position
	SDL_Rect rect;
	SDL_GetDisplayBounds(0, &rect);
	if (_isFullScreen)
	{
		_windowRect.width = rect.w;
		_windowRect.height = rect.h;
	}
	else
	{
		if (_isCenter)
		{
			_windowRect.x = rect.w / 2;
			_windowRect.y = rect.h / 2;
		}
	}
	SDL_SetWindowSize(_window, _windowRect.width, _windowRect.height);
	SDL_SetWindowPosition(_window, _windowRect.x - _windowRect.width / 2, _windowRect.y - _windowRect.height / 2);
	
	// Lock cursor in window
	if (_isGrab)
		SDL_SetWindowGrab(_window, SDL_TRUE);
	else
		SDL_SetWindowGrab(_window, SDL_FALSE);
}

void Graphics::CreateSamplePoint(int num)
{
	if (num)
	{
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, num);
	}
}

void Graphics::CreateDevice()
{
	_glContext = SDL_GL_CreateContext(_window);
	if (_glContext == NULL)
		ErrorString("Failed to create OpenGL context");
}

void Graphics::LoadAPILoader()
{
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		AssertString(-1, "Failed to initialize GLAD from Engine");
	}
}

bool Graphics::IsDeviceLost()
{
#if defined(IOS) || defined(TVOS)
	if (_window && (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED) != 0)
		return true;
#endif
	return _glContext == nullptr;
}

void Graphics::Draw(PrimitiveTypes type, unsigned vertexStart, unsigned vertexCount)
{
	if (!vertexCount)
		return;
	unsigned primitiveCount;
	GLenum glPrimitiveType;

	GetGLPrimitiveType(vertexCount, type, primitiveCount, glPrimitiveType);
	glDrawArrays(glPrimitiveType, vertexStart, vertexCount);

	_numPrimitives += primitiveCount;
	_numBatches++;
}

void Graphics::Draw(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount)
{
	//if (!indexCount || !indexBuffer_ || !indexBuffer_->GetGPUObjectName())
	//	return;


	//unsigned indexSize;// = _indexBuffer->GetIndexSize();
	//unsigned primitiveCount;
	//GLenum glPrimitiveType;

	//GetGLPrimitiveType(indexCount, type, primitiveCount, glPrimitiveType);
	//GLenum indexType = indexSize == sizeof(unsigned short) ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;
	//glDrawElements(glPrimitiveType, indexCount, indexType, reinterpret_cast<const GLvoid*>(indexStart * indexSize));

	//_numPrimitives += primitiveCount;
	//_numBatches++;
}

void Graphics::DrawInstanced(PrimitiveTypes type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,
	unsigned instanceCount)
{
	//glDrawElementsInstanced();
	//_numPrimitives += primitiveCount;
	//_numBatches++;
}


void Graphics::SetViewport(int posX, int posY, int width, int height)
{
	glViewport(posX, posY, width, height);
}


}
#endif //AUTO_OPENGL