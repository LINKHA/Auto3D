#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "../../Window/GLContext.h"
#include "../../Window/Window.h"
#include "../GPUObject.h"
#include "../Shader.h"
#include "OGLGraphics.h"
#include "OGLConstantBuffer.h"
#include "OGLIndexBuffer.h"
#include "OGLShaderProgram.h"
#include "OGLShaderVariation.h"
#include "OGLTexture.h"
#include "OGLVertexBuffer.h"

#include <cstdlib>
#include <glad.h>

#ifdef WIN32
#include <Windows.h>
// Prefer the high-performance GPU on switchable GPU systems
extern "C" {
    __declspec(dllexport) DWORD NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}
#endif

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

static const unsigned elementGLTypes[] =
{
    GL_INT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_FLOAT,
    GL_UNSIGNED_BYTE,
    GL_FLOAT,
    GL_FLOAT
};

static const unsigned elementGLComponents[] =
{
    1,
    1,
    2,
    3,
    4,
    4,
    12,
    16
};

static const unsigned glPrimitiveTypes[] =
{
	0,
	GL_POINTS,
	GL_LINES,
	GL_LINE_STRIP,
	GL_TRIANGLES,
	GL_TRIANGLE_STRIP
};

static const unsigned glBlendFactors[] =
{
    0,
    GL_ZERO,
    GL_ONE,
    GL_SRC_COLOR,
    GL_ONE_MINUS_SRC_COLOR,
    GL_SRC_ALPHA,
    GL_ONE_MINUS_SRC_ALPHA,
    GL_DST_ALPHA,
    GL_ONE_MINUS_DST_ALPHA,
    GL_DST_COLOR,
    GL_ONE_MINUS_DST_COLOR,
    GL_SRC_ALPHA_SATURATE,
};

static const unsigned glBlendOps[] =
{
    0,
    GL_FUNC_ADD,
    GL_FUNC_SUBTRACT,
    GL_FUNC_REVERSE_SUBTRACT,
    GL_MIN,
    GL_MAX
};

static const unsigned glCompareFuncs[] =
{
    0,
    GL_NEVER,
    GL_LESS,
    GL_EQUAL,
    GL_LEQUAL,
    GL_GREATER,
    GL_NOTEQUAL,
    GL_GEQUAL,
    GL_ALWAYS,
};

static const unsigned glStencilOps[] =
{
    0,
    GL_KEEP,
    GL_ZERO,
    GL_REPLACE,
    GL_INCR,
    GL_DECR,
    GL_INVERT,
    GL_INCR_WRAP,
    GL_DECR_WRAP,
};

static const unsigned glFillModes[] =
{
    0,
    0,
    GL_LINE,
    GL_FILL
};

static unsigned MAX_FRAMEBUFFER_AGE = 16;

/// OpenGL framebuffer.
class Framebuffer
{
public:
    /// Construct.
    Framebuffer() :
        depthStencil(nullptr),
        drawBuffers(0),
        firstUse(true)
    {
        glGenFramebuffers(1, &buffer);
        for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
            renderTargets[i] = nullptr;
    }

    /// Destruct.
    ~Framebuffer()
    {
        glDeleteFramebuffers(1, &buffer);
    }

    /// OpenGL FBO _handle.
    unsigned buffer;
    /// Color rendertargets bound to this FBO.
    Texture* renderTargets[MAX_RENDERTARGETS];
    /// Depth-stencil texture bound to this FBO.
    Texture* depthStencil;
    /// Enabled draw buffers.
    unsigned drawBuffers;
    /// First use flag; used for setting up readbuffers.
    bool firstUse;
};

Graphics::Graphics() :
    _backbufferSize(Vector2I::ZERO),
    _renderTargetSize(Vector2I::ZERO),
    _attributesBySemantic(ElementSemantic::Count),
    _multisample(1),
    _vsync(false)
{
    RegisterSubsystem(this);
    _window = new Window();
    SubscribeToEvent(_window->resizeEvent, &Graphics::HandleResize);
    ResetState();
}

Graphics::~Graphics()
{
    Close();
    RemoveSubsystem(this);
}

bool Graphics::SetMode(const RectI& _size,int multisample, bool fullscreen, bool resizable, bool center, bool borderless, bool highDPI)
{
    // Changing multisample requires destroying the _window, as OpenGL pixel format can only be set once
    if (!_context || multisample != _multisample)
    {
        bool recreate = false;

        if (IsInitialized())
        {
            recreate = true;
            Close();
            SendEvent(_contextLossEvent);
        }

        if (!_window->SetSize(_size, multisample, fullscreen, resizable, center, borderless, highDPI))
            return false;
        if (!CreateContext(multisample))
            return false;
		
        if (recreate)
        {
            // Recreate GPU objects that can be recreated
            for (auto it = _gpuObjects.Begin(); it != _gpuObjects.End(); ++it)
            {
                GPUObject* object = *it;
                object->Recreate();
            }
            SendEvent(_contextRestoreEvent);
        }
    }
    else
    {
        // If no context creation, just need to resize the _window
        if (!_window->SetSize(_size, fullscreen, resizable, center, borderless, highDPI))
            return false;
    }

    _backbufferSize = _window->GetSize();
    ResetRenderTargets();
    ResetViewport();
    // Cleanup framebuffers defined during the old resolution now
    CleanupFramebuffers();

    _screenModeEvent._size = _backbufferSize;
    _screenModeEvent._fullscreen = IsFullscreen();
    _screenModeEvent._resizable = IsResizable();
    _screenModeEvent._multisample = _multisample;
    SendEvent(_screenModeEvent);

    LogStringF("Set screen mode %dx%d fullscreen %d resizable %d multisample %d", _backbufferSize._x, _backbufferSize._y,
        IsFullscreen(), IsResizable(), _multisample);

    return true;
}

bool Graphics::SetFullscreen(bool enable)
{
	if (!IsInitialized())
		return false;
	else
		return SetMode(RectI(0, 0, _backbufferSize._x, _backbufferSize._y), enable, _window->IsResizable(), _multisample);
}

bool Graphics::SetMultisample(int multisample)
{
    if (!IsInitialized())
        return false;
    else
        return SetMode(RectI(0, 0, _backbufferSize._x, _backbufferSize._y), _window->IsFullscreen(), _window->IsResizable(), multisample);
}

void Graphics::SetVSync(bool enable)
{
    _vsync = enable;
    if (_context)
        _context->SetVSync(enable);
}

void Graphics::Close()
{
    _shaderPrograms.Clear();
    _framebuffers.Clear();

    // Release all GPU objects
    for (auto it = _gpuObjects.Begin(); it != _gpuObjects.End(); ++it)
    {
        GPUObject* object = *it;
        object->Release();
    }

    _context.Reset();

    ResetState();
}

void Graphics::Present()
{
    PROFILE(Present);

    _context->Present();

    // In case of third party hooks which modify the GL state and don't restore it properly, re-enable depth test now
    glEnable(GL_DEPTH_TEST);
	
}

void Graphics::SetRenderTarget(Texture* renderTarget, Texture* depthStencil)
{
    _renderTargetVector.Resize(1);
    _renderTargetVector[0] = renderTarget;
    SetRenderTargets(_renderTargetVector, depthStencil);
}

void Graphics::SetRenderTargets(const Vector<Texture*>& renderTargets, Texture* depthStencil)
{
    for (size_t i = 0; i < MAX_RENDERTARGETS && i < renderTargets.Size(); ++i)
        _renderTargets[i] = (renderTargets[i] && renderTargets[i]->IsRenderTarget()) ? renderTargets[i] : nullptr;

    for (size_t i = renderTargets.Size(); i < MAX_RENDERTARGETS; ++i)
        _renderTargets[i] = nullptr;

    _depthStencil = (depthStencil && depthStencil->IsDepthStencil()) ? depthStencil : nullptr;

    if (_renderTargets[0])
        _renderTargetSize = Vector2I(_renderTargets[0]->GetWidth(), _renderTargets[0]->GetHeight());
    else if (_depthStencil)
        _renderTargetSize = Vector2I(_depthStencil->GetWidth(), _depthStencil->GetHeight());
    else
        _renderTargetSize = _backbufferSize;

    _framebufferDirty = true;
}

void Graphics::SetViewport(const RectI& viewport)
{
    PrepareFramebuffer();

    /// \todo Implement a member function in IntRect for clipping
    _viewport.Left() = Clamp(viewport.Left(), 0, _renderTargetSize._x - 1);
    _viewport.Top() = Clamp(viewport.Top(), 0, _renderTargetSize._y - 1);
    _viewport.Right() = Clamp(viewport.Right(), _viewport.Left() + 1, _renderTargetSize._x);
    _viewport.Bottom() = Clamp(viewport.Bottom(), _viewport.Top() + 1, _renderTargetSize._y);

    // When rendering to the backbuffer, use Direct3D convention with the vertical coordinates ie. 0 is top
    if (!_framebuffer)
        glViewport(_viewport.Left(), _renderTargetSize._y - _viewport.Bottom(), _viewport.Width(), _viewport.Height());
    else
        glViewport(_viewport.Left(), _viewport.Top(), _viewport.Width(), _viewport.Height());
}

void Graphics::SetVertexBuffer(size_t index, VertexBuffer* buffer)
{
    if (index < MAX_VERTEX_STREAMS && buffer != _vertexBuffers[index])
    {
        _vertexBuffers[index] = buffer;
        _vertexBuffersDirty = true;
    }
}

void Graphics::SetIndexBuffer(IndexBuffer* buffer)
{
    if (_indexBuffer != buffer)
    {
        _indexBuffer = buffer;
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer ? buffer->GetGLBuffer() : 0);
    }
}

void Graphics::SetConstantBuffer(ShaderStage stage, size_t index, ConstantBuffer* buffer)
{
    if (stage < ShaderStage::Count && index < MAX_CONSTANT_BUFFERS && buffer != _constantBuffers[stage][index])
    {
        _constantBuffers[stage][index] = buffer;
        unsigned bufferObject = buffer ? buffer->GetGLBuffer() : 0;

        switch (stage)
        {
        case ShaderStage::VS:
            if (index < _vsConstantBuffers)
            {
                glBindBufferBase(GL_UNIFORM_BUFFER, (unsigned)index, bufferObject);
                _boundUBO = bufferObject;
            }
            break;

        case ShaderStage::PS:
            if (index < _psConstantBuffers)
            {
                glBindBufferBase(GL_UNIFORM_BUFFER, (unsigned)(index + _vsConstantBuffers), bufferObject);
                _boundUBO = bufferObject;
            }
            break;

        default:
            break;
        }
    }
}

void Graphics::SetTexture(size_t index, Texture* texture)
{
    if (index < MAX_TEXTURE_UNITS && texture != _textures[index])
    {
        _textures[index] = texture;
        
        if (index != _activeTexture)
        {
            glActiveTexture(GL_TEXTURE0 + (unsigned)index);
            _activeTexture = index;
        }

        if (texture)
        {
            unsigned target = texture->GetGLTarget();
            // Make sure we do not have multiple targets bound in the same unit
            if (_textureTargets[index] && _textureTargets[index] != target)
                glBindTexture(_textureTargets[index], 0);
            glBindTexture(target, texture->GetGLTexture());
            _textureTargets[index] = target;
        }
        else if (_textureTargets[index])
        {
            glBindTexture(_textureTargets[index], 0);
            _textureTargets[index] = 0;
        }
    }
}

void Graphics::SetShaders(ShaderVariation* vs, ShaderVariation* ps)
{
    if (vs == _vertexShader && ps == _pixelShader)
        return;

    if (vs != _vertexShader)
    {
        if (vs && vs->GetStage() == ShaderStage::VS)
        {
            if (!vs->IsCompiled())
                vs->Compile();
        }

        _vertexShader = vs;
    }

    if (ps != _pixelShader)
    {
        if (ps && ps->GetStage() == ShaderStage::PS)
        {
            if (!ps->IsCompiled())
                ps->Compile();
        }

        _pixelShader = ps;
    }

    if (_vertexShader && _pixelShader && _vertexShader->GetGLShader() && _pixelShader->GetGLShader())
    {
        // Check if program already exists, if not, link now
        auto key = MakePair(_vertexShader, _pixelShader);
        auto it = _shaderPrograms.Find(key);
        if (it != _shaderPrograms.End())
        {
            _shaderProgram = it->_second;
            glUseProgram(it->_second->GLProgram());
        }
        else
        {
            ShaderProgram* newProgram = new ShaderProgram(_vertexShader, _pixelShader);
            _shaderPrograms[key] = newProgram;
            // Note: if the linking is successful, glUseProgram() will have been called
            if (newProgram->Link())
                _shaderProgram = newProgram;
            else
            {
                _shaderProgram = nullptr;
                glUseProgram(0);
            }
        }
    }
    else
    {
        _shaderProgram = nullptr;
        glUseProgram(0);
    }

    _vertexAttributesDirty = true;
}

void Graphics::SetColorState(const BlendModeDesc& blendMode, bool alphaToCoverage, unsigned char colorWriteMask)
{
    _renderState._blendMode = blendMode;
    _renderState._colorWriteMask = colorWriteMask;
    _renderState._alphaToCoverage = alphaToCoverage;
    
    _blendStateDirty = true;
}

void Graphics::SetColorState(BlendMode blendMode, bool alphaToCoverage, unsigned char colorWriteMask)
{
    _renderState._blendMode = blendModes[blendMode];
    _renderState._colorWriteMask = colorWriteMask;
    _renderState._alphaToCoverage = alphaToCoverage;

    _blendStateDirty = true;
}

void Graphics::SetDepthState(CompareFunc depthFunc, bool depthWrite, bool depthClip, int depthBias, float slopeScaledDepthBias)
{
    _renderState._depthFunc = depthFunc;
    _renderState._depthWrite = depthWrite;
    _renderState._depthClip = depthClip;
    _renderState._depthBias = depthBias;
    _renderState._slopeScaledDepthBias = slopeScaledDepthBias;

    _depthStateDirty = true;
    _rasterizerStateDirty = true;
}

void Graphics::SetRasterizerState(CullMode cullMode, FillMode fillMode)
{
    _renderState._cullMode = cullMode;
    _renderState._fillMode = fillMode;

    _rasterizerStateDirty = true;
}

void Graphics::SetScissorTest(bool scissorEnable, const RectI& scissorRect)
{
    _renderState._scissorEnable = scissorEnable;
    /// \todo Implement a member function in IntRect for clipping
    _renderState._scissorRect.Left() = Clamp(scissorRect.Left(), 0, _renderTargetSize._x - 1);
    _renderState._scissorRect.Top() = Clamp(scissorRect.Top(), 0, _renderTargetSize._y - 1);
    _renderState._scissorRect.Right() = Clamp(scissorRect.Right(), _renderState._scissorRect.Left() + 1, _renderTargetSize._x);
    _renderState._scissorRect.Bottom() = Clamp(scissorRect.Bottom(), _renderState._scissorRect.Top() + 1, _renderTargetSize._y);

    _rasterizerStateDirty = true;
}

void Graphics::SetStencilTest(bool stencilEnable, const StencilTestDesc& stencilTest, unsigned char stencilRef)
{
    _renderState._stencilEnable = stencilEnable;
    _renderState._stencilTest = stencilTest;
    _renderState._stencilRef = stencilRef;

    _depthStateDirty = true;
}

void Graphics::ResetRenderTargets()
{
    SetRenderTarget(nullptr, nullptr);
}

void Graphics::ResetViewport()
{
    SetViewport(RectI(0, 0, _renderTargetSize._x, _renderTargetSize._y));
}

void Graphics::ResetVertexBuffers()
{
    for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
        SetVertexBuffer(i, nullptr);
}

void Graphics::ResetConstantBuffers()
{
    for (size_t i = 0; i < ShaderStage::Count; ++i)
    {
        for (size_t j = 0; i < MAX_CONSTANT_BUFFERS; ++j)
            SetConstantBuffer((ShaderStage)i, j, nullptr);
    }
}

void Graphics::ResetTextures()
{
    for (size_t i = 0; i < MAX_TEXTURE_UNITS; ++i)
        SetTexture(i, nullptr);
}

void Graphics::ResetGraphics()
{
	ResetViewport();
	ResetVertexBuffers();
	ResetConstantBuffers();
	ResetTextures();
	ResetRenderTargets();
	ResetViewport();
}

void Graphics::Clear(unsigned clearFlags, const Color& clearColor, float clearDepth, unsigned char clearStencil)
{
    PrepareFramebuffer();

    unsigned glFlags = 0;
    if (clearFlags & CLEAR_COLOR)
    {
        glFlags |= GL_COLOR_BUFFER_BIT;
        glClearColor(clearColor._r, clearColor._g, clearColor._b, clearColor._a);
    }
    if (clearFlags & CLEAR_DEPTH)
    {
        glFlags |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(clearDepth);
    }
    if (clearFlags & CLEAR_STENCIL)
    {
        glFlags |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(clearStencil);
    }

    if ((clearFlags & CLEAR_COLOR) && _glRenderState._colorWriteMask != COLORMASK_ALL)
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    if ((clearFlags & CLEAR_DEPTH) && !_glRenderState._depthWrite)
        glDepthMask(GL_TRUE);
    if ((clearFlags & CLEAR_STENCIL) && _glRenderState._stencilTest._stencilWriteMask != 0xff)
        glStencilMask(0xff);
    if (_glRenderState._scissorEnable)
        glDisable(GL_SCISSOR_TEST);

    glClear(glFlags);

    if ((clearFlags & CLEAR_COLOR) && _glRenderState._colorWriteMask != COLORMASK_ALL)
    {
        glColorMask(
            (_glRenderState._colorWriteMask & COLORMASK_R) ? GL_TRUE : GL_FALSE,
            (_glRenderState._colorWriteMask & COLORMASK_G) ? GL_TRUE : GL_FALSE,
            (_glRenderState._colorWriteMask & COLORMASK_B) ? GL_TRUE : GL_FALSE,
            (_glRenderState._colorWriteMask & COLORMASK_A) ? GL_TRUE : GL_FALSE
        );
    }
    if ((clearFlags & CLEAR_DEPTH) && !_glRenderState._depthWrite)
        glDepthMask(GL_FALSE);
    if ((clearFlags & CLEAR_STENCIL) && _glRenderState._stencilTest._stencilWriteMask != 0xff)
        glStencilMask(_glRenderState._stencilTest._stencilWriteMask);
    if (_glRenderState._scissorEnable)
        glEnable(GL_SCISSOR_TEST);
}

void Graphics::Draw(PrimitiveType type, size_t vertexStart, size_t vertexCount)
{
	if (!PrepareDraw())
        return;

    glDrawArrays(glPrimitiveTypes[type], (unsigned)vertexStart, (unsigned)vertexCount);
}

void Graphics::DrawIndexed(PrimitiveType type, size_t indexStart, size_t indexCount, size_t vertexStart)
{
    // Drawing with trashed index data can lead to a crash within the OpenGL driver
    if (!_indexBuffer || _indexBuffer->IsDataLost() || !PrepareDraw())
        return;
    
    size_t indexSize = _indexBuffer->IndexSize();

    if (!vertexStart)
    {
        glDrawElements(glPrimitiveTypes[type], (unsigned)indexCount, indexSize == sizeof(unsigned short) ? GL_UNSIGNED_SHORT :
            GL_UNSIGNED_INT, (const void*)(indexStart * indexSize));
    }
	else
	{
		glDrawElementsBaseVertex(glPrimitiveTypes[type], (unsigned)indexCount, indexSize == sizeof(unsigned short) ?
			GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (const void*)(indexStart * indexSize), (unsigned)vertexStart);
	}

}

void Graphics::DrawInstanced(PrimitiveType type, size_t vertexStart, size_t vertexCount, size_t instanceStart, size_t
    instanceCount)
{
    if (!PrepareDraw(true, instanceStart))
        return;

    glDrawArraysInstanced(glPrimitiveTypes[type], (unsigned)vertexStart, (unsigned)vertexCount, (unsigned)instanceCount);
}

void Graphics::DrawIndexedInstanced(PrimitiveType type, size_t indexStart, size_t indexCount, size_t vertexStart, size_t instanceStart,
    size_t instanceCount)
{
    if (!_indexBuffer || _indexBuffer->IsDataLost() || !PrepareDraw(true, instanceStart))
        return;

    size_t indexSize = _indexBuffer->IndexSize();
    
    if (!vertexStart)
    {
        glDrawElementsInstanced(glPrimitiveTypes[type], (unsigned)indexCount, indexSize == sizeof(unsigned short) ?
            GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (const void*)(indexStart * indexSize), (unsigned)instanceCount);
    }
    else
    {
        glDrawElementsInstancedBaseVertex(glPrimitiveTypes[type], (unsigned)indexCount, indexSize == sizeof(unsigned short) ?
            GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (const void*)(indexStart * indexSize), (unsigned)instanceCount, 
            (unsigned)vertexStart);
    }
}

bool Graphics::IsInitialized() const
{
    return _window->IsOpen() && _context;
}

bool Graphics::IsFullscreen() const
{
    return _window->IsFullscreen();
}

bool Graphics::IsResizable() const
{
    return _window->IsResizable();
}

Window* Graphics::RenderWindow() const
{
    return _window;
}

Texture* Graphics::RenderTarget(size_t index) const
{
    return index < MAX_RENDERTARGETS ? _renderTargets[index] : nullptr;
}

VertexBuffer* Graphics::GetVertexBuffer(size_t index) const
{
    return index < MAX_VERTEX_STREAMS ? _vertexBuffers[index] : nullptr;
}

ConstantBuffer* Graphics::GetConstantBuffer(ShaderStage stage, size_t index) const
{
    return (stage < ShaderStage::Count && index < MAX_CONSTANT_BUFFERS) ? _constantBuffers[stage][index] : nullptr;
}

Texture* Graphics::GetTexture(size_t index) const
{
    return (index < MAX_TEXTURE_UNITS) ? _textures[index] : nullptr;
}

void Graphics::AddGPUObject(GPUObject* object)
{
    if (object)
        _gpuObjects.Push(object);
}

void Graphics::RemoveGPUObject(GPUObject* object)
{
    /// \todo Requires a linear search, needs to be profiled whether becomes a problem with a large number of objects
    _gpuObjects.Remove(object);
}

void Graphics::CleanupShaderPrograms(ShaderVariation* shader)
{
    if (!shader)
        return;

    if (shader->GetStage() == ShaderStage::VS)
    {
        for (auto it = _shaderPrograms.Begin(); it != _shaderPrograms.End();)
        {
            if (it->_first._first == shader)
            {
                if (_shaderProgram == it->_second)
                    _shaderProgram = nullptr;
                it = _shaderPrograms.Erase(it);
            }
            else
                ++it;
        }
    }
    else
    {
        for (auto it = _shaderPrograms.Begin(); it != _shaderPrograms.End();)
        {
            if (it->_first._second == shader)
            {
                if (_shaderProgram == it->_second)
                    _shaderProgram = nullptr;
                it = _shaderPrograms.Erase(it);
            }
            else
                ++it;
        }
    }
}

void Graphics::CleanupFramebuffers(Texture* texture)
{
    if (!texture)
        return;

    for (auto it = _framebuffers.Begin(); it != _framebuffers.End(); ++it)
    {
        Framebuffer* framebuffer = it->_second;

        for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
        {
            if (framebuffer->renderTargets[i] == texture)
                framebuffer->renderTargets[i] = nullptr;
        }
        if (framebuffer->depthStencil == texture)
            framebuffer->depthStencil = nullptr;
    }
}

void Graphics::BindVBO(unsigned vbo)
{
    if (vbo != _boundVBO)
    {
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        _boundVBO = vbo;
    }
}

void Graphics::BindUBO(unsigned ubo)
{
    if (ubo != _boundUBO)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, ubo);
        _boundUBO = ubo;
    }
}

bool Graphics::CreateContext(int multisample)
{
    _context = new GLContext(_window);

    if (!_context->Create())
    {
        _context.Reset();
        return false;
    }

	int maxSamples;
	glGetIntegerv(GL_MAX_SAMPLES, &maxSamples);
	Clamp(multisample, 1, maxSamples);

    _multisample = multisample;
    _context->SetVSync(_vsync);

    // Query OpenGL capabilities
    int numBlocks;
    glGetIntegerv(GL_MAX_VERTEX_UNIFORM_BLOCKS, &numBlocks);
    _vsConstantBuffers = numBlocks;
    glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_BLOCKS, &numBlocks);
    _psConstantBuffers = numBlocks;

    // Create and bind a vertex array object that will stay in use throughout
    /// \todo Investigate performance gain of using multiple VAO's
    unsigned vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);

    // These states are always enabled to match Direct3D
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_POLYGON_OFFSET_LINE);
    glEnable(GL_POLYGON_OFFSET_FILL);

    // Set up texture data read/write alignment. It is important that this is done before uploading any texture data
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    return true;
}

void Graphics::HandleResize(WindowResizeEvent& event)
{
    // Reset viewport in case the application does not set it
    if (_context)
    {
        _backbufferSize = event._size;
        ResetRenderTargets();
        ResetViewport();
    }
}

void Graphics::CleanupFramebuffers()
{
    // Make sure the framebuffer is switched first if there are pending rendertarget changes
    PrepareFramebuffer();

    // Clear all except the framebuffer currently in use
    for (auto it = _framebuffers.Begin(); it != _framebuffers.End();)
    {
        if (it->_second != _framebuffer)
            it = _framebuffers.Erase(it);
        else
            ++it;
    }
}

void Graphics::PrepareFramebuffer()
{
    if (_framebufferDirty)
    {
        _framebufferDirty = false;
        
        unsigned newDrawBuffers = 0;
        bool useBackbuffer = true;

        // If rendertarget changes, scissor rect may need to be re-evaluated
        if (_renderState._scissorEnable)
        {
            _glRenderState._scissorRect = RectI::ZERO;
            _rasterizerStateDirty = true;
        }

        for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
        {
            if (_renderTargets[i])
            {
                useBackbuffer = false;
                newDrawBuffers |= (1 << i);
            }
        }
        if (_depthStencil)
            useBackbuffer = false;

        if (useBackbuffer)
        {
            if (_framebuffer)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                _framebuffer = nullptr;
            }
            return;
        }

        // Search for a new framebuffer based on format & _size, or create new
        ImageFormat _format = ImageFormat::NONE;
        if (_renderTargets[0])
            _format = _renderTargets[0]->GetFormat();
        else if (_depthStencil)
            _format = _depthStencil->GetFormat();
        unsigned long long key = (_renderTargetSize._x << 16 | _renderTargetSize._y) | (((unsigned long long)_format) << 32);
        
        auto it = _framebuffers.Find(key);
        if (it == _framebuffers.End())
            it = _framebuffers.Insert(MakePair(key, AutoPtr<Framebuffer>(new Framebuffer())));

        if (it->_second != _framebuffer)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, it->_second->buffer);
            _framebuffer = it->_second;
        }

        // Setup readbuffers & drawbuffers
        if (_framebuffer->firstUse)
        {
            glReadBuffer(GL_NONE);
            _framebuffer->firstUse = false;
        }

        if (newDrawBuffers != _framebuffer->drawBuffers)
        {
            if (!newDrawBuffers)
                glDrawBuffer(GL_NONE);
            else
            {
                int drawBufferIds[MAX_RENDERTARGETS];
                unsigned drawBufferCount = 0;

                for (unsigned i = 0; i < MAX_RENDERTARGETS; ++i)
                {
                    if (newDrawBuffers & (1 << i))
                        drawBufferIds[drawBufferCount++] = GL_COLOR_ATTACHMENT0 + i;
                }
                glDrawBuffers(drawBufferCount, (const GLenum*)drawBufferIds);
            }

            _framebuffer->drawBuffers = newDrawBuffers;
        }

        // Setup color attachments
        for (size_t i = 0; i < MAX_RENDERTARGETS; ++i)
        {
            if (_renderTargets[i] != _framebuffer->renderTargets[i])
            {
                if (_renderTargets[i])
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (unsigned)i, _renderTargets[i]->GetGLTarget(),
                        _renderTargets[i]->GetGLTexture(), 0);
                }
                else
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (unsigned)i, GL_TEXTURE_2D, 0, 0);
                
                _framebuffer->renderTargets[i] = _renderTargets[i];
            }
        }

        // Setup depth & stencil attachments
        if (_depthStencil != _framebuffer->depthStencil)
        {
            if (_depthStencil)
            {
                bool hasStencil = _depthStencil->GetFormat() == ImageFormat::D24S8;
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, _depthStencil->GetGLTarget(), 
                    _depthStencil->GetGLTexture(), 0);
                if (hasStencil)
                {
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, _depthStencil->GetGLTarget(),
                        _depthStencil->GetGLTexture(), 0);
                }
                else
                    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
            }
            else
            {
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
            }

            _framebuffer->depthStencil = _depthStencil;
        }
    }
}

bool Graphics::PrepareDraw(bool instanced, size_t instanceStart)
{
    if (_framebufferDirty)
        PrepareFramebuffer();

    if (!_shaderProgram)
        return false;

    if (_vertexAttributesDirty)
    {
        _usedVertexAttributes = 0;

        for (auto it = _attributesBySemantic.Begin(); it != _attributesBySemantic.End(); ++it)
            it->Clear();

        const Vector<VertexAttribute>& attributes = _shaderProgram->Attributes();
        for (auto it = attributes.Begin(); it != attributes.End(); ++it)
        {
            const VertexAttribute& attribute = *it;
            Vector<unsigned>& attributeVector = _attributesBySemantic[attribute._semantic];
            unsigned char index = attribute._index;

            // Mark semantic as required
            size_t _size = attributeVector.Size();
            if (_size <= index)
            {
                attributeVector.Resize(index + 1);
                // If there are gaps (eg. texcoord1 used without texcoord0), fill them with illegal index
                for (size_t j = _size; j < index; ++j)
                    attributeVector[j] = M_MAX_UNSIGNED;
            }
            attributeVector[index] = attribute._location;
            _usedVertexAttributes |= (1 << attribute._location);
        }

        _vertexAttributesDirty = false;
        _vertexBuffersDirty = true;
    }

    if (_vertexBuffersDirty || instanced)
    {
        // Now go through currently bound vertex buffers and set the attribute pointers that are available & required
        for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
        {
            if (_vertexBuffers[i])
            {
                VertexBuffer* buffer = _vertexBuffers[i];
                const Vector<VertexElement>& elements = buffer->GetElements();

                for (auto it = elements.Begin(); it != elements.End(); ++it)
                {
                    const VertexElement& element = *it;
                    const Vector<unsigned>& attributeVector = _attributesBySemantic[element._semantic];

                    // If making several instanced draw calls with the same vertex buffers, only need to update the instancing
                    // data attribute pointers
                    if (element._index < attributeVector.Size() && attributeVector[element._index] < M_MAX_UNSIGNED &&
                        (_vertexBuffersDirty || (instanced && element._perInstance)))
                    {
                        unsigned location = attributeVector[element._index];
                        unsigned locationMask = 1 << location;

                        // Enable attribute if not enabled yet
                        if (!(_enabledVertexAttributes & locationMask))
                        {
                            glEnableVertexAttribArray(location);
                            _enabledVertexAttributes |= locationMask;
                        }

                        // Enable/disable instancing divisor as necessary
                        size_t dataStart = element._offset;
                        if (element._perInstance)
                        {
                            dataStart += instanceStart * buffer->GetVertexSize();
                            if (!(_instancingVertexAttributes & locationMask))
                            {
                                glVertexAttribDivisor(location, 1);
                                _instancingVertexAttributes |= locationMask;
                            }
                        }
                        else
                        {
                            if (_instancingVertexAttributes & locationMask)
                            {
								glVertexAttribDivisor(location, 0);
                                _instancingVertexAttributes &= ~locationMask;
                            }
                        }

                        BindVBO(buffer->GetGLBuffer());
                        glVertexAttribPointer(location, elementGLComponents[element._type], elementGLTypes[element._type],
                            element._semantic == ElementSemantic::COLOR ? GL_TRUE : GL_FALSE, (unsigned)buffer->GetVertexSize(),
                            (const void *)dataStart);
                    }
                }
            }
        }

        _vertexBuffersDirty = false;
    }

    // Finally disable unnecessary vertex attributes
    unsigned disableVertexAttributes = _enabledVertexAttributes & (~_usedVertexAttributes);
    unsigned location = 0;
    while (disableVertexAttributes)
    {
        if (disableVertexAttributes & 1)
        {
            glDisableVertexAttribArray(location);
            _enabledVertexAttributes &= ~(1 << location);
        }
        ++location;
        disableVertexAttributes >>= 1;
    }

    // Apply blend state
    if (_blendStateDirty)
    {
        if (_renderState._blendMode._blendEnable != _glRenderState._blendMode._blendEnable)
        {
            if (_renderState._blendMode._blendEnable)
                glEnable(GL_BLEND);
            else
                glDisable(GL_BLEND);
            _glRenderState._blendMode._blendEnable = _renderState._blendMode._blendEnable;
        }

        if (_renderState._blendMode._blendEnable)
        {
            if (_renderState._blendMode._srcBlend != _glRenderState._blendMode._srcBlend || _renderState._blendMode._destBlend !=
                _glRenderState._blendMode._destBlend || _renderState._blendMode._srcBlendAlpha != _glRenderState._blendMode._srcBlendAlpha ||
                _renderState._blendMode._destBlendAlpha != _glRenderState._blendMode._destBlendAlpha)
            {
                glBlendFuncSeparate(glBlendFactors[_renderState._blendMode._srcBlend], glBlendFactors[_renderState._blendMode._destBlend],
                    glBlendFactors[_renderState._blendMode._srcBlendAlpha], glBlendFactors[_renderState._blendMode._destBlendAlpha]);
                _glRenderState._blendMode._srcBlend = _renderState._blendMode._srcBlend;
                _glRenderState._blendMode._destBlend = _renderState._blendMode._destBlend;
                _glRenderState._blendMode._srcBlendAlpha = _renderState._blendMode._srcBlendAlpha;
                _glRenderState._blendMode._destBlendAlpha = _renderState._blendMode._destBlendAlpha;
            }

            if (_renderState._blendMode._blendOp != _glRenderState._blendMode._blendOp || _renderState._blendMode._blendOpAlpha !=
                _glRenderState._blendMode._blendOpAlpha)
            {
                glBlendEquationSeparate(glBlendOps[_renderState._blendMode._blendOp], glBlendOps[_renderState._blendMode._blendOpAlpha]);
                _glRenderState._blendMode._blendOp = _renderState._blendMode._blendOp;
                _glRenderState._blendMode._blendOpAlpha = _renderState._blendMode._blendOpAlpha;
            }
        }

        if (_renderState._colorWriteMask != _glRenderState._colorWriteMask)
        {
            glColorMask(
                (_renderState._colorWriteMask & COLORMASK_R) ? GL_TRUE : GL_FALSE,
                (_renderState._colorWriteMask & COLORMASK_G) ? GL_TRUE : GL_FALSE,
                (_renderState._colorWriteMask & COLORMASK_B) ? GL_TRUE : GL_FALSE,
                (_renderState._colorWriteMask & COLORMASK_A) ? GL_TRUE : GL_FALSE
            );
            _glRenderState._colorWriteMask = _renderState._colorWriteMask;
        }

        if (_renderState._alphaToCoverage != _glRenderState._alphaToCoverage)
        {
            if (_renderState._alphaToCoverage)
                glEnable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            else
                glDisable(GL_SAMPLE_ALPHA_TO_COVERAGE);
            _glRenderState._alphaToCoverage = _renderState._alphaToCoverage;
        }

        _blendStateDirty = false;
    }

    // Apply depth state
    if (_depthStateDirty)
    {
        if (_renderState._depthWrite != _glRenderState._depthWrite)
        {
            glDepthMask(_renderState._depthWrite ? GL_TRUE : GL_FALSE);
            _glRenderState._depthWrite = _renderState._depthWrite;
        }

        if (_renderState._depthFunc != _glRenderState._depthFunc)
        {
            glDepthFunc(glCompareFuncs[_renderState._depthFunc]);
            _glRenderState._depthFunc = _renderState._depthFunc;
        }

        if (_renderState._stencilEnable != _glRenderState._stencilEnable)
        {
            if (_renderState._stencilEnable)
                glEnable(GL_STENCIL_TEST);
            else
                glDisable(GL_STENCIL_TEST);
            _glRenderState._stencilEnable = _renderState._stencilEnable;
        }

        if (_renderState._stencilEnable)
        {
            // Note: as polygons use Direct3D convention (clockwise = front) reversed front/back faces are used here
            if (_renderState._stencilTest._frontFunc != _glRenderState._stencilTest._frontFunc || _renderState._stencilRef !=
                _glRenderState._stencilRef || _renderState._stencilTest._stencilReadMask != _glRenderState._stencilTest._stencilReadMask)
            {
                glStencilFuncSeparate(GL_BACK, glCompareFuncs[_renderState._stencilTest._frontFunc], _renderState._stencilRef,
                    _renderState._stencilTest._stencilReadMask);
                _glRenderState._stencilTest._frontFunc = _renderState._stencilTest._frontFunc;
            }
            if (_renderState._stencilTest._backFunc != _glRenderState._stencilTest._backFunc || _renderState._stencilRef !=
                _glRenderState._stencilRef || _renderState._stencilTest._stencilReadMask != _glRenderState._stencilTest._stencilReadMask)
            {
                glStencilFuncSeparate(GL_BACK, glCompareFuncs[_renderState._stencilTest._backFunc], _renderState._stencilRef,
                    _renderState._stencilTest._stencilReadMask);
                _glRenderState._stencilTest._frontFunc = _renderState._stencilTest._frontFunc;
            }
            _glRenderState._stencilRef = _renderState._stencilRef;
            _glRenderState._stencilTest._stencilReadMask = _renderState._stencilTest._stencilReadMask;

            if (_renderState._stencilTest._stencilWriteMask != _glRenderState._stencilTest._stencilWriteMask)
            {
                glStencilMask(_renderState._stencilTest._stencilWriteMask);
                _glRenderState._stencilTest._stencilWriteMask = _renderState._stencilTest._stencilWriteMask;
            }

            if (_renderState._stencilTest._frontFail != _glRenderState._stencilTest._frontFail ||
                _renderState._stencilTest._frontDepthFail != _glRenderState._stencilTest._frontDepthFail ||
                _renderState._stencilTest._frontPass != _glRenderState._stencilTest._frontPass)
            {
                glStencilOpSeparate(GL_BACK, glStencilOps[_renderState._stencilTest._frontFail],
                    glStencilOps[_renderState._stencilTest._frontDepthFail], glStencilOps[_renderState._stencilTest._frontPass]);
                _glRenderState._stencilTest._frontFail = _renderState._stencilTest._frontFail;
                _glRenderState._stencilTest._frontDepthFail = _renderState._stencilTest._frontDepthFail;
                _glRenderState._stencilTest._frontPass = _renderState._stencilTest._frontPass;
            }
            if (_renderState._stencilTest._backFail != _glRenderState._stencilTest._backFail || _renderState._stencilTest._backDepthFail !=
                _glRenderState._stencilTest._backDepthFail || _renderState._stencilTest._backPass != _glRenderState._stencilTest._backPass)
            {
                glStencilOpSeparate(GL_FRONT, glStencilOps[_renderState._stencilTest._backFail], 
                    glStencilOps[_renderState._stencilTest._backDepthFail], glStencilOps[_renderState._stencilTest._backPass]);
                _glRenderState._stencilTest._backFail = _renderState._stencilTest._backFail;
                _glRenderState._stencilTest._backDepthFail = _renderState._stencilTest._backDepthFail;
                _glRenderState._stencilTest._backPass = _renderState._stencilTest._backPass;
            }
        }

        _depthStateDirty = false;
    }

    // Apply rasterizer state
    if (_rasterizerStateDirty)
    {
        if (_renderState._fillMode != _glRenderState._fillMode)
        {
            glPolygonMode(GL_FRONT_AND_BACK, glFillModes[_renderState._fillMode]);
            _glRenderState._fillMode = _renderState._fillMode;
        }

        if (_renderState._cullMode != _glRenderState._cullMode)
        {
            if (_renderState._cullMode == CullMode::NONE)
                glDisable(GL_CULL_FACE);
            else
            {
                if (_glRenderState._cullMode == CullMode::NONE)
                    glEnable(GL_CULL_FACE);
                // Note: as polygons use Direct3D convention (clockwise = front) reversed front/back faces are used here
                glCullFace(_renderState._cullMode == CullMode::BACK ? GL_FRONT : GL_BACK);
            }
            _glRenderState._cullMode = _renderState._cullMode;
        }

        if (_renderState._depthBias != _glRenderState._depthBias || _renderState._slopeScaledDepthBias !=
            _glRenderState._slopeScaledDepthBias)
        {
            /// \todo Check if this matches Direct3D
            glPolygonOffset(_renderState._slopeScaledDepthBias, (float)_renderState._depthBias);
            _glRenderState._depthBias = _renderState._depthBias;
            _glRenderState._slopeScaledDepthBias = _renderState._slopeScaledDepthBias;
        }

        if (_renderState._depthClip != _glRenderState._depthClip)
        {
            if (_renderState._depthClip)
                glDisable(GL_DEPTH_CLAMP);
            else
                glEnable(GL_DEPTH_CLAMP);
            _glRenderState._depthClip = _renderState._depthClip;
        }

        if (_renderState._scissorEnable != _glRenderState._scissorEnable)
        {
            if (_renderState._scissorEnable)
                glEnable(GL_SCISSOR_TEST);
            else
                glDisable(GL_SCISSOR_TEST);
            _glRenderState._scissorEnable = _renderState._scissorEnable;
        }

        if (_renderState._scissorEnable && _renderState._scissorRect != _glRenderState._scissorRect)
        {
            glScissor(_renderState._scissorRect.Left(), _renderTargetSize._y - _renderState._scissorRect.Bottom(),
                _scissorRect.Width(), _scissorRect.Height());
            _glRenderState._scissorRect = _renderState._scissorRect;
        }

        _rasterizerStateDirty = false;
    }

    return true;
}

void Graphics::ResetState()
{
    for (size_t i = 0; i < MAX_VERTEX_STREAMS; ++i)
        _vertexBuffers[i] = nullptr;

    _enabledVertexAttributes = 0;
    _usedVertexAttributes = 0;
    _instancingVertexAttributes = 0;

    for (size_t i = 0; i < ShaderStage::Count; ++i)
    {
        for (size_t j = 0; j < MAX_CONSTANT_BUFFERS; ++j)
            _constantBuffers[i][j] = nullptr;
    }

    for (size_t i = 0; i < MAX_TEXTURE_UNITS; ++i)
    {
        _textures[i] = nullptr;
        _textureTargets[i] = 0;
    }

    _indexBuffer = nullptr;
    _vertexShader = nullptr;
    _pixelShader = nullptr;
    _shaderProgram = nullptr;
    _framebuffer = nullptr;
    _vertexAttributesDirty = false;
    _vertexBuffersDirty = false;
    _blendStateDirty = false;
    _depthStateDirty = false;
    _rasterizerStateDirty = false;
    _framebufferDirty = false;
    _activeTexture = 0;
    _boundVBO = 0;
    _boundUBO = 0;

    _glRenderState._depthWrite = false;
    _glRenderState._depthFunc = CompareFunc::ALWAYS;
    _glRenderState._depthBias = 0;
    _glRenderState._slopeScaledDepthBias = 0;
    _glRenderState._depthClip = true;
    _glRenderState._colorWriteMask = COLORMASK_ALL;
    _glRenderState._alphaToCoverage = false;
    _glRenderState._blendMode._blendEnable = false;
    _glRenderState._blendMode._srcBlend = BlendFactor::Count;
    _glRenderState._blendMode._destBlend = BlendFactor::Count;
    _glRenderState._blendMode._blendOp = BlendOp::Count;
    _glRenderState._blendMode._srcBlendAlpha = BlendFactor::Count;
    _glRenderState._blendMode._destBlendAlpha = BlendFactor::Count;
    _glRenderState._blendMode._blendOpAlpha = BlendOp::Count;
    _glRenderState._fillMode = FillMode::SOLID;
    _glRenderState._cullMode = CullMode::NONE;
    _glRenderState._scissorEnable = false;
    _glRenderState._scissorRect = RectI::ZERO;
    _glRenderState._stencilEnable = false;
    _glRenderState._stencilRef = 0;
    _glRenderState._stencilTest._stencilReadMask = 0xff;
    _glRenderState._stencilTest._stencilWriteMask = 0xff;
    _glRenderState._stencilTest._frontFail = StencilOp::KEEP;
    _glRenderState._stencilTest._frontDepthFail = StencilOp::KEEP;
    _glRenderState._stencilTest._frontPass = StencilOp::KEEP;
    _glRenderState._stencilTest._frontFunc = CompareFunc::ALWAYS;
    _glRenderState._stencilTest._backFail = StencilOp::KEEP;
    _glRenderState._stencilTest._backDepthFail = StencilOp::KEEP;
    _glRenderState._stencilTest._backPass = StencilOp::KEEP;
    _glRenderState._stencilTest._backFunc = CompareFunc::ALWAYS;
    _renderState = _glRenderState;
}

void RegisterGraphicsLibrary()
{
    static bool registered = false;
    if (registered)
        return;
    registered = true;

    Shader::RegisterObject();
    Texture::RegisterObject();
}

}
