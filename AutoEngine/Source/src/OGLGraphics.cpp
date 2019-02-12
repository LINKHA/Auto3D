#include "Graphics.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "OGLGraphicsImp.h"
#include "IndexBuffer.h"

#include "NewDef.h"

namespace Auto3D {
static const unsigned glCmpFunc[] =
{
	GL_ALWAYS,
	GL_EQUAL,
	GL_NOTEQUAL,
	GL_LESS,
	GL_LEQUAL,
	GL_GREATER,
	GL_GEQUAL
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

static const unsigned glElementTypes[] =
{
	GL_INT,
	GL_FLOAT,
	GL_UNSIGNED_BYTE
};

void APIENTRY glDebugOutput(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	void *userParam)
{
	// ignore non-significant error/warning codes
	if (
		id == 131169 || id == 131185 || id == 131218 || id == 131204 || id || // driver-specific non-significant error codes
		id == 2000 || id == 2001 || id == 2265 // shader compilation error codes; ignore as already managed from shader object
		)
	{
		return;
	}

	std::string logMessage = "Debug output: (" + std::to_string(id) + "): " + message + "\n";

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             logMessage += "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   logMessage += "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: logMessage += "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     logMessage += "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     logMessage += "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           logMessage += "Source: Other"; break;
	} logMessage += "\n";

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               logMessage += "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: logMessage += "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  logMessage += "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         logMessage += "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         logMessage += "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              logMessage += "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          logMessage += "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           logMessage += "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               logMessage += "Type: Other"; break;
	} logMessage += "\n";

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         logMessage += "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       logMessage += "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          logMessage += "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: logMessage += "Severity: notification"; break;
	} logMessage += "\n";
	logMessage += "\n";

	// only log a message a maximum of 3 times (as it'll keep spamming the message queue with
	// the same error message)
	static unsigned int msgCount = 0;
	if (msgCount++ < 3)
	{
		WarningString(logMessage);
	}
}

static void GetGLPrimitiveType(unsigned elementCount, PrimitiveType type, unsigned& primitiveCount, GLenum& glPrimitiveType)
{
	switch (type)
	{
	case PrimitiveType::TringleList:
		primitiveCount = elementCount / 3;
		glPrimitiveType = GL_TRIANGLES;
		break;

	case PrimitiveType::LineList:
		primitiveCount = elementCount / 2;
		glPrimitiveType = GL_LINES;
		break;

	case PrimitiveType::PointList:
		primitiveCount = elementCount;
		glPrimitiveType = GL_POINTS;
		break;

	case PrimitiveType::TriangleStrip:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_STRIP;
		break;

	case PrimitiveType::LineStrip:
		primitiveCount = elementCount - 1;
		glPrimitiveType = GL_LINE_STRIP;
		break;

	case PrimitiveType::TiangleFan:
		primitiveCount = elementCount - 2;
		glPrimitiveType = GL_TRIANGLE_FAN;
		break;
	}
}

Graphics::Graphics(SharedPtr<Ambient> ambient)
	:Super(ambient)
	, _window(nullptr)
	, _impl(MakeShared<GraphicsImpl>())
	, _shadowMapFormat(GL_DEPTH_COMPONENT16)
#if _OPENGL_4_6_
	, _apiName("OpenGL 4.6")
#elif _OPENGL_4_PLUS_
	, _apiName("OpenGL 4.3")
#elif _OPENGL_3_PLUS_
	, _apiName("OpenGL 3.3")
#elif _DIRECT_3D_12
	, _apiName("Direct3D 12")
#else
	, _apiName("UnKnow")
#endif
{
	ResetCachedState();

	RegisterGraphicsLib(_ambient);
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
		glDebugMessageCallback((GLDEBUGPROC)glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
}

void Graphics::Init()
{
	_icon = GetSubSystem<ResourceSystem>()->GetResource<Image>("texture/logo.png");

	stbi_set_flip_vertically_on_load(true);
	// Create game window
	CreateGameWindow();
	// Create device(OpenGL context and DirectX device)
	CreateDevice();
	// Register graphics API debug
	RegisterDebug();
	// Create Icon
	CreateIcon();

	Restore();

	InitGraphicsState();

}

void Graphics::DestoryWindow()
{

	SDL_GL_DeleteContext(_impl->_glContext);
	_impl->_glContext = nullptr;

	SDL_DestroyWindow(_window);
	_window = nullptr;
	SDL_Quit();
}

void Graphics::ReleaseAPI()
{
	glDeleteVertexArrays(1, &_vertexArrayObject);
}
void Graphics::Restore()
{
	if (!_window)
		return;
	// Create and bind a vertex array object that will stay in use throughout
	glGenVertexArrays(1, &_vertexArrayObject);
	glBindVertexArray(_vertexArrayObject);
}

void Graphics::SetVBO(unsigned object)
{
	if (_impl->boundVBO_ != object)
	{
		if (object)
			glBindBuffer(GL_ARRAY_BUFFER, object);
		_impl->boundVBO_ = object;
	}
}

void Graphics::ResetCachedState()
{
	_drawColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	_viewport = RectInt(0, 0, 0, 0);
	_colorWrite = false;
	_depthWrite = false;
	_stencilWriteMask = MATH_MAX_UNSIGNED;
	_numPrimitives = 0;
	_numBatches = 0;
	_numSample = 0;
	_depthTestMode = DepthMode::Always;
}
void Graphics::InitGraphicsState()
{
	SetDepthTest(DepthMode::Less);
	SetColorWrite(true);
	SetDepthWrite(true);
}
void Graphics::SetIndexBuffer(SharedPtr<IndexBuffer> buffer)
{
	if (_indexBuffer == buffer)
		return;

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer ? buffer->GetHandle().name : 0);
	_indexBuffer = buffer;
}

bool Graphics::BeginFrame()
{
	if (!IsInitialized() || IsDeviceLost())
		return false;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(glCmpFunc[static_cast<unsigned>(_depthTestMode)]);
	SetColorWrite(true);
	SetDepthWrite(true);
	Clear(CLEAR_TARGET_COLOR | CLEAR_TARGET_DEPTH | CLEAR_TARGET_STENCIL);
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

	if (flags & CLEAR_TARGET_COLOR && !oldColorWrite)
		SetColorWrite(true);
	if (flags & CLEAR_TARGET_DEPTH && !oldDepthWrite)
		SetDepthWrite(true);
	if (flags & CLEAR_TARGET_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(MATH_MAX_UNSIGNED);

	unsigned glFlags = 0;
	if (flags & CLEAR_TARGET_COLOR)
	{
		glFlags |= GL_COLOR_BUFFER_BIT;
		glClearColor(color.r, color.g, color.b, color.a);
	}
	if (flags & CLEAR_TARGET_DEPTH)
	{
		glFlags |= GL_DEPTH_BUFFER_BIT;
		glClearDepth(depth);
	}
	if (flags & CLEAR_TARGET_STENCIL)
	{
		glFlags |= GL_STENCIL_BUFFER_BIT;
		glClearStencil(stencil);
	}
	glClear(glFlags);
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
		_titleName.CStr(),
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
	_impl->_glContext = SDL_GL_CreateContext(_window);
	if (_impl->_glContext == NULL)
		ErrorString("Failed to create OpenGL context");
	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
	{
		AssertString(-1, "Failed to initialize GLAD from Engine");
	}
	else
	{
		// Set driver name
		_driverName = STRING((char*)glGetString(GL_VENDOR)) + STRING((char*)glGetString(GL_RENDERER));
	}
}

bool Graphics::IsDeviceLost()
{
#if defined(IOS) || defined(TVOS)
	if (_window && (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED) != 0)
		return true;
#endif
	return _impl->_glContext == nullptr;
}

void Graphics::Draw(PrimitiveType type, unsigned vertexStart, unsigned vertexCount)
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

void Graphics::Draw(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount)
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

void Graphics::DrawInstanced(PrimitiveType type, unsigned indexStart, unsigned indexCount, unsigned minVertex, unsigned vertexCount,
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

void Graphics::SetDepthTest(DepthMode mode)
{
	if (mode != _depthTestMode)
	{
		glDepthFunc(glCmpFunc[static_cast<unsigned>(mode)]);
		_depthTestMode = mode;
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


}
#endif //AUTO_OPENGL