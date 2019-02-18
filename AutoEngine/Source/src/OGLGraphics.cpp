#include "Graphics.h"
#if AUTO_OPENGL
#include "AutoOGL.h"
#include "AutoImage.h"
#include "ResourceSystem.h"
#include "Image.h"
#include "OGLGraphicsImp.h"
#include "IndexBuffer.h"
#include "Texture.h"
#include "RenderSurface.h"
#include "VertexBuffer.h"
#include "Texture2D.h"
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
	, _impl(MakeShared<GraphicsImpl>())
	, _shadowMapFormat(GL_DEPTH_COMPONENT16)
	, _hiresShadowMapFormat(GL_DEPTH_COMPONENT24)
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

	CheckFeatureSupport();
	
	InitGraphicsState();

	Clear(CLEAR_TARGET_COLOR | CLEAR_TARGET_DEPTH | CLEAR_TARGET_STENCIL);
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

void Graphics::CheckFeatureSupport()
{
	// Check supported features: light pre-pass, deferred rendering and hardware depth texture
	_lightPrepassSupport = false;
	_deferredSupport = false;

	int numSupportedRTs = 1;

	// Work around GLEW failure to check extensions properly from a GL3 context
	_instancingSupport = glDrawElementsInstanced != nullptr && glVertexAttribDivisor != nullptr;
	_dxtTextureSupport = true;
	_anisotropySupport = true;
	_sRGBSupport = true;
	_sRGBWriteSupport = true;

	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &numSupportedRTs);

	// Must support 2 rendertargets for light pre-pass, and 4 for deferred
	if (numSupportedRTs >= 2)
		_lightPrepassSupport = true;
	if (numSupportedRTs >= 4)
		_deferredSupport = true;

	// Consider OpenGL shadows always hardware sampled, if supported at all
	_hardwareShadowSupport = _shadowMapFormat != 0;
}
void Graphics::SetVBO(unsigned object)
{
	if (_impl->_boundVBO != object)
	{
		if (object)
			glBindBuffer(GL_ARRAY_BUFFER, object);
		_impl->_boundVBO = object;
	}
}

void Graphics::SetUBO(unsigned object)
{
	if (_impl->_boundUBO != object)
	{
		if (object)
			glBindBuffer(GL_UNIFORM_BUFFER, object);
		_impl->_boundUBO = object;
	}
}

void Graphics::ResetCachedState()
{
	for (size_t i =0; i<MAX_VERTEX_STREAMS; i++)
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

	//_numPrimitives = 0;
	//_numBatches = 0;
	//_numSample = 0;

	//_drawColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	//_viewport = RectInt(0, 0, 0, 0);
	//_colorWrite = false;
	//_depthWrite = false;
	//_stencilWriteMask = MATH_MAX_UNSIGNED;
	//_scissorRect = RectInt(0, 0, 0, 0);
	//_depthTestMode = DepthMode::Always;
	//_blendMode = BlendMode::Replace;
	//_alphaToCoverage = false;

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
	_currentRenderState = _renderState;

	//// Set initial state to match Direct3D
	//if (_impl->_glContext)
	//{
	//	glEnable(GL_DEPTH_TEST);
	//	SetCullMode(CullMode::CCW);
	//	SetDepthTest(DepthMode::LessEqual);
	//	SetDepthWrite(true);
	//}
}

void Graphics::MarkFBODirty()
{
	_impl->_fboDirty = true;
}

void Graphics::ResetRenderTargets()
{
	for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
		SetRenderTarget(i, SharedPtr<RenderSurface>());
	SetDepthStencil(SharedPtr<RenderSurface>());
	SetViewport(RectInt(0, 0, _windowRect.width, _windowRect.height));
}

void Graphics::ResetRenderTarget(unsigned index)
{
	SetRenderTarget(index, SharedPtr<RenderSurface>());
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

void Graphics::SetBlendMode(BlendMode mode, bool alphaToCoverage)
{
	if (mode != _blendMode)
	{
		if (mode == BlendMode::Replace)
			glDisable(GL_BLEND);
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(glSrcBlend[(int)mode], glDestBlend[(int)mode]);
			glBlendEquation(glBlendOp[(int)mode]);
		}

		_blendMode = mode;
	}

	if (alphaToCoverage != _alphaToCoverage)
	{
		if (alphaToCoverage)
			glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
		else
			glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);

		_alphaToCoverage = alphaToCoverage;
	}
}

bool Graphics::BeginFrame()
{
	if (!IsInitialized() || IsDeviceLost())
		return false;

	// Re-enable depth test and depth func in case a third party program has modified it
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(glCmpFunc[(unsigned)_depthTestMode]);

	ResetRenderTargets();

	// Cleanup textures from previous frame
	for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
		SetTexture(i, SharedPtr<Texture>());

	SetColorWrite(true);
	SetDepthWrite(true);

	_numPrimitives = 0;
	_numBatches = 0;

	//Clear(CLEAR_TARGET_COLOR | CLEAR_TARGET_DEPTH | CLEAR_TARGET_STENCIL);
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
	PrepareDraw();

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
	// If viewport is less than full screen, set a scissor to limit the clear
	/// todo Any user-set scissor test will be lost
	Vector2 viewSize = GetRenderTargetDimensions();
	if (_viewport.width != viewSize.x || _viewport.height != viewSize.y)
		SetScissorTest(true, RectInt(0, 0, _viewport.width, _viewport.height));
	else
		SetScissorTest(false);

	glClear(glFlags);

	SetScissorTest(false);
	SetColorWrite(oldColorWrite);
	SetDepthWrite(oldDepthWrite);
	if (flags & CLEAR_TARGET_STENCIL && _stencilWriteMask != MATH_MAX_UNSIGNED)
		glStencilMask(_stencilWriteMask);
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

SharedPtr<RenderSurface> Graphics::GetRenderTarget(unsigned index) const
{
	return index < MAX_RENDERTARGETS ? _renderTargets[index] : nullptr;
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

void Graphics::PrepareDraw()
{
	for (VECTOR<SharedPtr<ConstantBuffer> >::iterator i = _impl->_dirtyConstantBuffers.begin(); i != _impl->_dirtyConstantBuffers.end(); ++i)
		(*i)->Apply();
	_impl->_dirtyConstantBuffers.clear();

	if (_impl->_fboDirty)
	{
		_impl->_fboDirty = false;

		// First check if no framebuffer is needed. In that case simply return to backbuffer rendering
		bool noFbo = !_depthStencil;
		if (noFbo)
		{
			for (auto& renderTarget : _renderTargets)
			{
				if (renderTarget)
				{
					noFbo = false;
					break;
				}
			}
		}

		if (noFbo)
		{
			if (_impl->_boundFBO != _impl->_systemFBO)
			{
				BindFramebuffer(_impl->_systemFBO);
				_impl->_boundFBO = _impl->_systemFBO;
			}

			// Disable/enable sRGB write
			if (_sRGBWriteSupport)
			{
				bool sRGBWrite = _sRGB;
				if (sRGBWrite != _impl->_sRGBWrite)
				{
					if (sRGBWrite)
						glEnable(GL_FRAMEBUFFER_SRGB);
					else
						glDisable(GL_FRAMEBUFFER_SRGB);
					_impl->_sRGBWrite = sRGBWrite;
				}
			}
			return;
		}


		// Search for a new framebuffer based on format & size, or create new
		Vector2 rtSize = Graphics::GetRenderTargetDimensions();
		unsigned format = 0;
		if (_renderTargets[0])
			format = _renderTargets[0]->GetParentTexture()->GetFormat();
		else if (_depthStencil)
			format = _depthStencil->GetParentTexture()->GetFormat();

		auto fboKey = (unsigned long long)format << 32u | (int)rtSize.x << 16u | (int)rtSize.y;
		HASH_MAP<unsigned long long, FrameBufferObject>::iterator i = _impl->_frameBuffers.find(fboKey);
		if (i == _impl->_frameBuffers.end())
		{
			FrameBufferObject newFbo;
			newFbo._fbo = CreateFramebuffer();
			i = _impl->_frameBuffers.insert(i, MakePair(fboKey, newFbo));
		}

		if (_impl->_boundFBO != i->second._fbo)
		{
			BindFramebuffer(i->second._fbo);
			_impl->_boundFBO = i->second._fbo;
		}
		// Setup readbuffers & drawbuffers if needed
		if (i->second._readBuffers != GL_NONE)
		{
			glReadBuffer(GL_NONE);
			i->second._readBuffers = GL_NONE;
		}

		// Calculate the bit combination of non-zero color rendertargets to first check if the combination changed
		unsigned newDrawBuffers = 0;
		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (_renderTargets[j])
				newDrawBuffers |= 1u << j;
		}

		if (newDrawBuffers != i->second._drawBuffers)
		{
			// Check for no color rendertargets (depth rendering only)
			if (!newDrawBuffers)
				glDrawBuffer(GL_NONE);
			else
			{
				int drawBufferIds[MAX_RENDERTARGETS];
				unsigned drawBufferCount = 0;

				for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
				{
					if (_renderTargets[j])
					{
						drawBufferIds[drawBufferCount++] = GL_COLOR_ATTACHMENT0 + j;
					}
				}
				glDrawBuffers(drawBufferCount, (const GLenum*)drawBufferIds);
			}

			i->second._drawBuffers = newDrawBuffers;
		}

		for (unsigned j = 0; j < MAX_RENDERTARGETS; ++j)
		{
			if (_renderTargets[j])
			{
				SharedPtr<Texture> texture = _renderTargets[j]->GetParentTexture();

				// Bind either a renderbuffer or texture, depending on what is available
				unsigned renderBufferID = _renderTargets[j]->GetRenderBuffer();
				if (!renderBufferID)
				{
					// If texture's parameters are dirty, update before attaching
					if (texture->GetParametersDirty())
					{
						SetTextureForUpdate(texture);
						texture->UpdateParameters();
						SetTexture(0, nullptr);
					}

					if (i->second._colorAttachments[j] != _renderTargets[j])
					{
						BindColorAttachment(j, _renderTargets[j]->GetTarget(), texture->GetHandle().name, false);
						i->second._colorAttachments[j] = _renderTargets[j];
					}
				}
				else
				{
					if (i->second._colorAttachments[j] != _renderTargets[j])
					{
						BindColorAttachment(j, _renderTargets[j]->GetTarget(), renderBufferID, true);
						i->second._colorAttachments[j] = _renderTargets[j];
					}
				}
			}
			else
			{
				if (i->second._colorAttachments[j])
				{
					BindColorAttachment(j, GL_TEXTURE_2D, 0, false);
					i->second._colorAttachments[j] = nullptr;
				}
			}
		}

		if (_depthStencil)
		{
			// Bind either a renderbuffer or a depth texture, depending on what is available
			SharedPtr<Texture> texture = _depthStencil->GetParentTexture();

			bool hasStencil = texture->GetFormat() == GL_DEPTH24_STENCIL8;

			unsigned renderBufferID = _depthStencil->GetRenderBuffer();
			if (!renderBufferID)
			{
				// If texture's parameters are dirty, update before attaching
				if (texture->GetParametersDirty())
				{
					SetTextureForUpdate(texture);
					texture->UpdateParameters();
					SetTexture(0, nullptr);
				}

				if (i->second._depthAttachment != _depthStencil)
				{
					BindDepthAttachment(texture->GetHandle().name, false);
					BindStencilAttachment(hasStencil ? texture->GetHandle().name : 0, false);
					i->second._depthAttachment = _depthStencil;
				}
			}
			else
			{
				if (i->second._depthAttachment != _depthStencil)
				{
					BindDepthAttachment(renderBufferID, true);
					BindStencilAttachment(hasStencil ? renderBufferID : 0, true);
					i->second._depthAttachment = _depthStencil;
				}
			}
		}
		else
		{
			if (i->second._depthAttachment)
			{
				BindDepthAttachment(0, false);
				BindStencilAttachment(0, false);
				i->second._depthAttachment = nullptr;
			}
		}

		// Disable/enable sRGB write
		if (_sRGBWriteSupport)
		{
			bool sRGBWrite = _renderTargets[0] ? _renderTargets[0]->GetParentTexture()->GetSRGB() : _sRGB;
			if (sRGBWrite != _impl->_sRGBWrite)
			{
				if (sRGBWrite)
					glEnable(GL_FRAMEBUFFER_SRGB);
				else
					glDisable(GL_FRAMEBUFFER_SRGB);
				_impl->_sRGBWrite = sRGBWrite;
			}
		}
	}

	if (_impl->_vertexBuffersDirty)
	{
		// Go through currently bound vertex buffers and set the attribute pointers that are available & required
		// Use reverse order so that elements from higher index buffers will override lower index buffers
		unsigned assignedLocations = 0;

		for (unsigned i = MAX_VERTEX_STREAMS - 1; i < MAX_VERTEX_STREAMS; --i)
		{
			SharedPtr<VertexBuffer> buffer = _vertexBuffers[i];
			// Beware buffers with missing OpenGL objects, as binding a zero buffer object means accessing CPU memory for vertex data,
			// in which case the pointer will be invalid and cause a crash
			if (!buffer || !buffer->GetHandle().name || _impl->_vertexAttributes.empty())
				continue;

			const VECTOR<VertexElement>& elements = buffer->GetElements();

			for (VECTOR<VertexElement>::const_iterator j = elements.begin(); j != elements.end(); ++j)
			{
				const VertexElement& element = *j;
				PAIR_MAP<PAIR<unsigned char, unsigned char>, unsigned>::const_iterator k =
					_impl->_vertexAttributes.find(MakePair((unsigned char)element._semantic, element._index));

				if (k != _impl->_vertexAttributes.end())
				{
					unsigned location = k->second;
					unsigned locationMask = 1u << location;
					if (assignedLocations & locationMask)
						continue; // Already assigned by higher index vertex buffer
					assignedLocations |= locationMask;

					// Enable attribute if not enabled yet
					if (!(_impl->_enabledVertexAttributes & locationMask))
					{
						glEnableVertexAttribArray(location);
						_impl->_enabledVertexAttributes |= locationMask;
					}

					// Enable/disable instancing divisor as necessary
					unsigned dataStart = element._offset;
					if (element._perInstance)
					{
						dataStart += _impl->_lastInstanceOffset * buffer->GetVertexSize();
						if (!(_impl->_instancingVertexAttributes & locationMask))
						{
							SetVertexAttribDivisor(location, 1);
							_impl->_instancingVertexAttributes |= locationMask;
						}
					}
					else
					{
						if (_impl->_instancingVertexAttributes & locationMask)
						{
							SetVertexAttribDivisor(location, 0);
							_impl->_instancingVertexAttributes &= ~locationMask;
						}
					}

					SetVBO(buffer->GetHandle().name);
					glVertexAttribPointer(location, glElementComponents[(int)element._type], glElementTypes[(int)element._type],
						element._type == VertexElementType::Ubyte4Norm ? GL_TRUE : GL_FALSE, (unsigned)buffer->GetVertexSize(),
						(const void *)(size_t)dataStart);
				}
			}
		}

		// Finally disable unnecessary vertex attributes
		unsigned disableVertexAttributes = _impl->_enabledVertexAttributes & (~_impl->_usedVertexAttributes);
		unsigned location = 0;
		while (disableVertexAttributes)
		{
			if (disableVertexAttributes & 1u)
			{
				glDisableVertexAttribArray(location);
				_impl->_enabledVertexAttributes &= ~(1u << location);
			}
			++location;
			disableVertexAttributes >>= 1;
		}

		_impl->_vertexBuffersDirty = false;
	}
}

void Graphics::DeleteFramebuffer(unsigned fbo)
{
	glDeleteFramebuffers(1, &fbo);
}

void Graphics::BindFramebuffer(unsigned fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void Graphics::BindDepthAttachment(unsigned object, bool isRenderBuffer)
{
	if (!object)
		isRenderBuffer = false;

	
	if (!isRenderBuffer)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, object, 0);
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, object);
	
}

void Graphics::BindStencilAttachment(unsigned object, bool isRenderBuffer)
{
	if (!object)
		isRenderBuffer = false;


	if (!isRenderBuffer)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, object, 0);
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, object);

}

void Graphics::SetDefaultTextureFilterMode(TextureFilterMode mode)
{
	if (mode != _defaultTextureFilterMode)
	{
		_defaultTextureFilterMode = mode;
		SetTextureParametersDirty();
	}
}

void Graphics::SetDefaultTextureAnisotropy(unsigned level)
{
	level = max(level, 1U);

	if (level != _defaultTextureAnisotropy)
	{
		_defaultTextureAnisotropy = level;
		SetTextureParametersDirty();
	}
}

void Graphics::SetTextureParametersDirty()
{

	for (VECTOR<GPUObject*>::iterator i = _gpuObjects.begin(); i != _gpuObjects.end(); ++i)
	{
		auto* texture = dynamic_cast<Texture*>(*i);
		if (texture)
			texture->SetParametersDirty();
	}
}

void Graphics::SetVertexAttribDivisor(unsigned location, unsigned divisor)
{
	if (_instancingSupport)
		glVertexAttribDivisor(location, divisor);
}

void Graphics::SetStencilTest(bool enable, StencilMode mode, StencilOp pass, StencilOp fail, StencilOp zFail, unsigned stencilRef,
	unsigned compareMask, unsigned writeMask)
{
	if (enable != _stencilTest)
	{
		if (enable)
			glEnable(GL_STENCIL_TEST);
		else
			glDisable(GL_STENCIL_TEST);
		_stencilTest = enable;
	}

	if (enable)
	{
		if (mode != _stencilTestMode || stencilRef != _stencilRef || compareMask != _stencilCompareMask)
		{
			glStencilFunc(glCmpFunc[(int)mode], stencilRef, compareMask);
			_stencilTestMode = mode;
			_stencilRef = stencilRef;
			_stencilCompareMask = compareMask;
		}
		if (writeMask != _stencilWriteMask)
		{
			glStencilMask(writeMask);
			_stencilWriteMask = writeMask;
		}
		if (pass != _stencilPass || fail != _stencilFail || zFail != _stencilZFail)
		{
			glStencilOp(glStencilOps[(int)fail], glStencilOps[(int)zFail], glStencilOps[(int)pass]);
			_stencilPass = pass;
			_stencilFail = fail;
			_stencilZFail = zFail;
		}
	}
}

void Graphics::BindColorAttachment(unsigned index, unsigned target, unsigned object, bool isRenderBuffer)
{
	if (!object)
		isRenderBuffer = false;

	if (!isRenderBuffer)
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target, object, 0);
	else
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_RENDERBUFFER, object);

}

Vector2 Graphics::GetRenderTargetDimensions() const
{
	int width, height;

	if (_renderTargets[0])
	{
		width = _renderTargets[0]->GetWidth();
		height = _renderTargets[0]->GetHeight();
	}
	else if (_depthStencil)
	{
		width = _depthStencil->GetWidth();
		height = _depthStencil->GetHeight();
	}
	else
	{
		width = _windowRect.width;
		height = _windowRect.height;
	}

	return Vector2(width, height);
}

void Graphics::SetRenderTarget(unsigned index, SharedPtr<RenderSurface> renderTarget)
{
	if (index >= MAX_RENDERTARGETS)
		return;

	if (renderTarget != _renderTargets[index])
	{
		_renderTargets[index] = renderTarget;

		// If the rendertarget is also bound as a texture, replace with backup texture or null
		if (renderTarget)
		{
			SharedPtr<Texture> parentTexture = renderTarget->GetParentTexture();

			for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
			{
				if (_textures[i] == parentTexture)
					SetTexture(i, _textures[i]->GetBackupTexture());
			}

			// If multisampled, mark the texture & surface needing resolve
			if (parentTexture->GetMultiSample() > 1 && parentTexture->GetAutoResolve())
			{
				parentTexture->SetResolveDirty(true);
				renderTarget->SetResolveDirty(true);
			}

			// If mipmapped, mark the levels needing regeneration
			if (parentTexture->GetLevels() > 1)
				parentTexture->SetLevelsDirty();
		}

		_impl->_fboDirty = true;
	}
}

void Graphics::SetRenderTarget(unsigned index, SharedPtr<Texture2D> texture)
{

}

void Graphics::SetDepthStencil(SharedPtr<RenderSurface> depthStencil)
{
	if (_renderTargets[0] && _renderTargets[0]->GetMultiSample() == 1 && !depthStencil)
	{
		int width = _renderTargets[0]->GetWidth();
		int height = _renderTargets[0]->GetHeight();

		// Direct3D9 default depth-stencil can not be used when rendertarget is larger than the window.
		// Check size similarly
		if (width <= _windowRect.width && height <= _windowRect.height)
		{
			unsigned searchKey = (width << 16u) | height;
			HASH_MAP<unsigned, SharedPtr<Texture2D> >::iterator i = _impl->_depthTextures.find(searchKey);
			if (i != _impl->_depthTextures.end())
				depthStencil = i->second->GetRenderSurface();
			else
			{
				SharedPtr<Texture2D> newDepthTexture = MakeShared<Texture2D>(_ambient);
				newDepthTexture->SetSize(width, height, GetDepthStencilFormat(), TextureUsage::DepthStencil);
				_impl->_depthTextures[searchKey] = newDepthTexture;
				depthStencil = newDepthTexture->GetRenderSurface();
			}
		}
	}

	if (depthStencil != _depthStencil)
	{
		_depthStencil = depthStencil;
		_impl->_fboDirty = true;
	}
}

void Graphics::SetDepthStencil(SharedPtr<Texture2D> texture)
{
	SharedPtr<RenderSurface> depthStencil;
	if (texture)
		depthStencil = texture->GetRenderSurface();

	SetDepthStencil(depthStencil);
}

void Graphics::SetViewport(const RectInt& rect)
{
	PrepareDraw();

	Vector2 rtSize = GetRenderTargetDimensions();

	RectInt rectCopy = rect;

	if (rectCopy.width <= 0)
		rectCopy.width = 1;
	if (rectCopy.height <= 0)
		rectCopy.height = 1;
	rectCopy.x = clamp(rectCopy.x, 0, (int)rtSize.x);
	rectCopy.y = clamp(rectCopy.y, 0, (int)rtSize.y);


	// Use Direct3D convention with the vertical coordinates ie. 0 is top
	glViewport(rectCopy.x, rtSize.y, rectCopy.width, rectCopy.height);
	_viewport = rectCopy;

	// Disable scissor test, needs to be re-enabled by the user
	SetScissorTest(false);
}

void Graphics::SetTexture(unsigned index, SharedPtr<Texture> texture)
{
	if (index >= MAX_TEXTURE_UNITS)
	{
		WarningString("Index exceeds maximum texture limit.");
		return;
	}

	// Check if texture is currently bound as a rendertarget. In that case, use its backup texture, or blank if not defined
	if (texture)
	{
		if (_renderTargets[0] && _renderTargets[0]->GetParentTexture() == texture)
			texture = texture->GetBackupTexture();
		else
		{
			// Resolve multisampled texture now as necessary
			if (texture->GetMultiSample() > 1 && texture->GetAutoResolve() && texture->IsResolveDirty())
			{
				if (texture->GetClassString() == Texture2D::GetClassStringStatic())
					ResolveToTexture(StaticCast<Texture2D>(texture));
				/*if (texture->GetType() == TextureCube::GetTypeStatic())
					ResolveToTexture(StaticCast<TextureCube>(texture));*/
			}
		}
	}
	if (_textures[index] != texture)
	{
		if (_impl->_activeTexture != index)
		{
			glActiveTexture(GL_TEXTURE0 + index);
			_impl->_activeTexture = index;
		}

		if (texture)
		{
			unsigned glType = texture->GetTarget();
			// Unbind old texture type if necessary
			if (_impl->_textureTypes[index] && _impl->_textureTypes[index] != glType)
				glBindTexture(_impl->_textureTypes[index], 0);
			glBindTexture(glType, texture->GetHandle().name);
			_impl->_textureTypes[index] = glType;

			if (texture->GetParametersDirty())
				texture->UpdateParameters();
			if (texture->GetLevelsDirty())
				texture->RegenerateLevels();
		}
		else if (_impl->_textureTypes[index])
		{
			glBindTexture(_impl->_textureTypes[index], 0);
			_impl->_textureTypes[index] = 0;
		}

		_textures[index] = texture;
	}
	else
	{
		if (texture && (texture->GetParametersDirty() || texture->GetLevelsDirty()))
		{
			if (_impl->_activeTexture != index)
			{
				glActiveTexture(GL_TEXTURE0 + index);
				_impl->_activeTexture = index;
			}

			glBindTexture(texture->GetTarget(), texture->GetHandle().name);
			if (texture->GetParametersDirty())
				texture->UpdateParameters();
			if (texture->GetLevelsDirty())
				texture->RegenerateLevels();
		}
	}
}

unsigned Graphics::CreateFramebuffer()
{
	unsigned newFbo = 0;
	glGenFramebuffers(1, &newFbo);
	return newFbo;
}

bool Graphics::ResolveToTexture(SharedPtr<Texture2D> texture)
{
	if (!texture)
		return false;
	SharedPtr<RenderSurface> surface = texture->GetRenderSurface();
	if (!surface || !surface->GetRenderBuffer())
		return false;

	texture->SetResolveDirty(false);
	surface->SetResolveDirty(false);

	// Use separate FBOs for resolve to not disturb the currently set rendertarget(s)
	if (!_impl->_resolveSrcFBO)
		_impl->_resolveSrcFBO = CreateFramebuffer();
	if (!_impl->_resolveDestFBO)
		_impl->_resolveDestFBO = CreateFramebuffer();


	glBindFramebuffer(GL_READ_FRAMEBUFFER, _impl->_resolveSrcFBO);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, surface->GetRenderBuffer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _impl->_resolveDestFBO);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture->GetHandle().name, 0);
	glBlitFramebuffer(0, 0, texture->GetWidth(), texture->GetHeight(), 0, 0, texture->GetWidth(), texture->GetHeight(),
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	
	// Restore previously bound FBO
	BindFramebuffer(_impl->_boundFBO);
	return true;
}

void Graphics::SetCullMode(CullMode mode)
{
	if (mode != _cullMode)
	{
		if (mode == CullMode::None)
			glDisable(GL_CULL_FACE);
		else
		{
			// Use Direct3D convention, ie. clockwise vertices define a front face
			glEnable(GL_CULL_FACE);
			glCullFace(mode == CullMode::CCW ? GL_FRONT : GL_BACK);
		}

		_cullMode = mode;
	}
}


void Graphics::SetDepthTest(DepthMode mode)
{
	if (mode != _depthTestMode)
	{
		glDepthFunc(glCmpFunc[static_cast<unsigned>(mode)]);
		_depthTestMode = mode;
	}
}

void Graphics::SetScissorTest(bool enable, const RectInt& rect, bool borderInclusive)
{
	// During some light rendering loops, a full rect is toggled on/off repeatedly.
	// Disable scissor in that case to reduce state changes

	if (enable)
	{
		Vector2 rtSize(GetRenderTargetDimensions());
		Vector2 viewSize(_viewport.GetSize());
		Vector2 viewPos(_viewport.x, _viewport.y);
		RectInt intRect;

		if (enable && _scissorRect != intRect)
		{
			// Use Direct3D convention with the vertical coordinates ie. 0 is top
			glScissor(intRect.x, intRect.y, intRect.width, intRect.height);
			_scissorRect = intRect;
		}
	}
	else
		_scissorRect = RectInt(0, 0, 0, 0);

	if (enable != _scissorTest)
	{
		if (enable)
			glEnable(GL_SCISSOR_TEST);
		else
			glDisable(GL_SCISSOR_TEST);
		_scissorTest = enable;
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
void Graphics::SetTextureForUpdate(SharedPtr<Texture> texture)
{
	if (_impl->_activeTexture != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		_impl->_activeTexture = 0;
	}

	unsigned glType = texture->GetTarget();
	// Unbind old texture type if necessary
	if (_impl->_textureTypes[0] && _impl->_textureTypes[0] != glType)
		glBindTexture(_impl->_textureTypes[0], 0);
	glBindTexture(glType, texture->GetHandle().name);
	_impl->_textureTypes[0] = glType;
	_textures[0] = texture;
}

void Graphics::SetSRGB(bool enable)
{
	enable &= _sRGBWriteSupport;

	if (enable != _sRGB)
	{
		_sRGB = enable;
		_impl->_fboDirty = true;
	}
}

unsigned Graphics::GetFormat(CompressedFormat format) const
{
	switch (format)
	{
	case CompressedFormat::RGBA:
		return GL_RGBA;
	case CompressedFormat::DXT1:
		return _dxtTextureSupport ? GL_COMPRESSED_RGBA_S3TC_DXT1_EXT : 0;
	case CompressedFormat::DXT3:
		return _dxtTextureSupport ? GL_COMPRESSED_RGBA_S3TC_DXT3_EXT : 0;
	case CompressedFormat::DXT5:
		return _dxtTextureSupport ? GL_COMPRESSED_RGBA_S3TC_DXT5_EXT : 0;
	default:
		return 0;
	}
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