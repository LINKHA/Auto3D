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

void Graphics::ResetCachedState()
{
	for (auto& vertexBuffer : _vertexBuffers)
		vertexBuffer.reset();

	for (unsigned i = 0; i < MAX_TEXTURE_UNITS; ++i)
	{
		_textures[i].reset();
		_impl->_textureTypes[i] = 0;
	}
	for (auto& renderTarget : _renderTargets)
		renderTarget.reset();

	_drawColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
	_viewport = RectInt(0, 0, 0, 0);
	_colorWrite = false;
	_depthWrite = false;
	_stencilWriteMask = MATH_MAX_UNSIGNED;
	_numPrimitives = 0;
	_numBatches = 0;
	_numSample = 0;
	_impl->_boundFBO = _impl->_systemFBO;
	_impl->_boundVBO = 0;
	_impl->_boundUBO = 0;
	_depthTestMode = DepthMode::Always;

	// Set initial state to match Direct3D
	if (_impl->_glContext)
	{
		glEnable(GL_DEPTH_TEST);
		SetCullMode(CullMode::CCW);
		SetDepthTest(DepthMode::LessEqual);
		SetDepthWrite(true);
	}
}

void Graphics::MarkFBODirty()
{
	_impl->_fboDirty = true;
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


void Graphics::SetViewport(int posX, int posY, int width, int height)
{
	glViewport(posX, posY, width, height);
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

void Graphics::BindFramebuffer(unsigned fbo)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
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