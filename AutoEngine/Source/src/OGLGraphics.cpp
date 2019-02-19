#include "Graphics.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "VertexBuffer.h"
#include "Image.h"
#include "ConstantBuffer.h"

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
static const unsigned glElementComponents[] =
{
	1,
	1,
	2,
	3,
	4,
	4,
	4
};

static const unsigned glSrcBlend[] =
{
	GL_ONE,
	GL_ONE,
	GL_DST_COLOR,
	GL_SRC_ALPHA,
	GL_SRC_ALPHA,
	GL_ONE,
	GL_ONE_MINUS_DST_ALPHA,
	GL_ONE,
	GL_SRC_ALPHA
};

static const unsigned glDestBlend[] =
{
	GL_ZERO,
	GL_ONE,
	GL_ZERO,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_ONE,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE,
	GL_ONE
};

static const unsigned glBlendOp[] =
{
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_ADD,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT
};

static const unsigned glStencilOps[] =
{
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR_WRAP,
	GL_DECR_WRAP
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

}

void Graphics::DestoryWindow()
{

	SDL_GL_DeleteContext(_glContext);
	_glContext = nullptr;

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

void Graphics::ResetCachedState()
{
	/*for (size_t i =0; i<MAX_VERTEX_STREAMS; i++)
		_vertexBuffers[i].reset();

	for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
	{
		_textures[i].reset();
		_impl->_textureTypes[i] = 0;
	}
	for (auto& renderTarget : _renderTargets)
		renderTarget.reset();

	_indexBuffer.reset();
	_vertexShader.reset();
	_pixelShader.reset();
	_shaderProgram.reset();


	_impl->_boundFBO = _impl->_systemFBO;
	_impl->_boundVBO = 0;
	_impl->_boundUBO = 0;
	_impl->_sRGBWrite = false;

	_renderState.depthWrite = false;
	_renderState.depthFunc = CompareFunc::Always;
	_renderState.depthBias = 0;
	_renderState.slopeScaledDepthBias = 0;
	_renderState.depthClip = true;
	_renderState.colorWriteMask = COLORMASK_ALL;
	_renderState.alphaToCoverage = false;
	_renderState.blendMode.blendEnable = false;
	_renderState.blendMode.srcBlend = BlendFactor::Count;
	_renderState.blendMode.destBlend = BlendFactor::Count;
	_renderState.blendMode.blendOp = BlendOp::Count;
	_renderState.blendMode.srcBlendAlpha = BlendFactor::Count;
	_renderState.blendMode.destBlendAlpha = BlendFactor::Count;
	_renderState.blendMode.blendOpAlpha = BlendOp::Count;
	_renderState.fillMode = FillMode::Solid;
	_renderState.cullMode = CullMode::None;
	_renderState.scissorEnable = false;
	_renderState.scissorRect = RectInt(0, 0, 0, 0);
	_renderState.stencilEnable = false;
	_renderState.stencilRef = 0;
	_renderState.stencilTest.stencilReadMask = 0xff;
	_renderState.stencilTest.stencilWriteMask = 0xff;
	_renderState.stencilTest.frontFail = StencilOp::Keep;
	_renderState.stencilTest.frontDepthFail = StencilOp::Keep;
	_renderState.stencilTest.frontPass = StencilOp::Keep;
	_renderState.stencilTest.frontFunc = CompareFunc::Always;
	_renderState.stencilTest.backFail = StencilOp::Keep;
	_renderState.stencilTest.backDepthFail = StencilOp::Keep;
	_renderState.stencilTest.backPass = StencilOp::Keep;
	_renderState.stencilTest.backFunc = CompareFunc::Always;
	_currentRenderState = _renderState;*/

}


bool Graphics::BeginFrame()
{

	return true;
}
void Graphics::EndFrame()
{
	if (!IsInitialized())
		return;
	SDL_GL_SwapWindow(_window);
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
	//CreateSamplePoint(_numSample);

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
		_windowRect._right = rect.w;
		_windowRect._top = rect.h;
		_windowRect._left = 0;
		_windowRect._bottom = 0;

	}
	else
	{
		if (_isCenter)
		{
			int w = _windowRect.Width();
			int h = _windowRect.Height();
			_windowRect._left = (rect.w - w) / 2;
			_windowRect._bottom = (rect.h - h) / 2;
			_windowRect._right = (rect.w + w) / 2;
			_windowRect._top = (rect.h + h) / 2;
		}
		SDL_SetWindowSize(_window, _windowRect.Width(), _windowRect.Height());
		SDL_SetWindowPosition(_window, _windowRect._left, _windowRect._bottom);

	}

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
	return _glContext == nullptr;
}

unsigned Graphics::GetAlphaFormat()
{
	return GL_R8;
}

unsigned Graphics::GetLuminanceFormat()
{
	return GL_R8;
}

unsigned Graphics::GetLuminanceAlphaFormat()
{
	return GL_RG8;
}

unsigned Graphics::GetRGBFormat()
{
	return GL_RGB;
}

unsigned Graphics::GetRGBAFormat()
{
	return GL_RGBA;
}

unsigned Graphics::GetRGBA16Format()
{
	return GL_RGBA16;
}

unsigned Graphics::GetRGBAFloat16Format()
{
	return GL_RGBA16F;
}

unsigned Graphics::GetRGBAFloat32Format()
{
	return GL_RGBA32F;
}

unsigned Graphics::GetRG16Format()
{
	return GL_RG16;
}

unsigned Graphics::GetRGFloat16Format()
{
	return GL_RG16F;
}

unsigned Graphics::GetRGFloat32Format()
{
	return GL_RG32F;
}

unsigned Graphics::GetFloat16Format()
{
	return GL_R16F;
}

unsigned Graphics::GetFloat32Format()
{
	return GL_R32F;
}

unsigned Graphics::GetLinearDepthFormat()
{
	return GL_R32F;
}

unsigned Graphics::GetDepthStencilFormat()
{
	return GL_DEPTH24_STENCIL8;
}

unsigned Graphics::GetReadableDepthFormat()
{
	return GL_DEPTH_COMPONENT24;
}

unsigned Graphics::GetFormat(const STRING& formatName)
{
	STRING nameLower = formatName.ToLower().Trimmed();

	if (nameLower == "a")
		return GetAlphaFormat();
	if (nameLower == "l")
		return GetLuminanceFormat();
	if (nameLower == "la")
		return GetLuminanceAlphaFormat();
	if (nameLower == "rgb")
		return GetRGBFormat();
	if (nameLower == "rgba")
		return GetRGBAFormat();
	if (nameLower == "rgba16")
		return GetRGBA16Format();
	if (nameLower == "rgba16f")
		return GetRGBAFloat16Format();
	if (nameLower == "rgba32f")
		return GetRGBAFloat32Format();
	if (nameLower == "rg16")
		return GetRG16Format();
	if (nameLower == "rg16f")
		return GetRGFloat16Format();
	if (nameLower == "rg32f")
		return GetRGFloat32Format();
	if (nameLower == "r16f")
		return GetFloat16Format();
	if (nameLower == "r32f" || nameLower == "float")
		return GetFloat32Format();
	if (nameLower == "lineardepth" || nameLower == "depth")
		return GetLinearDepthFormat();
	if (nameLower == "d24s8")
		return GetDepthStencilFormat();
	if (nameLower == "readabledepth" || nameLower == "hwdepth")
		return GetReadableDepthFormat();

	return GetRGBFormat();
}

}
#endif //AUTO_OPENGL