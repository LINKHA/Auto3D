#include "AutoConfig.h"
#if AUTO_VULKAN
#include "../../Debug/Log.h"
#include "../../Debug/Profiler.h"
#include "VKGraphicsContext.h"
#include "../../Window/Window.h"
#include "../GPUObject.h"
#include "../Shader.h"
#include "VKGraphics.h"
#include "VKConstantBuffer.h"
#include "VKIndexBuffer.h"
#include "VKShaderProgram.h"
#include "VKShaderVariation.h"
#include "VKTexture.h"
#include "VKVertexBuffer.h"

#include <cstdlib>
#include <vulkan/vulkan.h>

#if defined(WIN32)
#	include <Windows.h>
#endif

#include "../../Debug/DebugNew.h"

namespace Auto3D
{

Graphics::Graphics() :
	_backbufferSize(Vector2I::ZERO),
	_renderTargetSize(Vector2I::ZERO),
	_attributesBySemantic(ElementSemantic::Count),
	_multisample(1),
	_graphicsVersion(GraphicsVersion::VULKAN_1_0),
	_graphicsSLVersion(GraphicsSLVersion::GLSL_450),
	_vsync(false)
{
	RegisterSubsystem(this);
	_window = new Window();
	SubscribeToEvent(_window->resizeEvent, &Graphics::HandleResize);
	ResetState();
}

Graphics::~Graphics()
{

}

void Graphics::CheckFeatureSupport()
{

}

bool Graphics::SetMode(const RectI& size, int multisample, bool fullscreen, bool resizable, bool center, bool borderless, bool highDPI)
{
	// Ensure that MSAA between 1~16
	Clamp(multisample, 1, 16);
	// Changing multisample requires destroying the _window, as OpenGL pixel format can only be set once
	if (!_context || multisample != _multisample)
	{
		bool recreate = false;

		if (IsInitialized())
		{
			recreate = true;
			Close();
			if (_window)
				_window->DestoryWindow();

			SendEvent(_contextLossEvent);
		}

		if (!_window->InitMsg())
		{
			ErrorString("Failed to initialize the form");
			return false;
		}
		if (!_window->SetSize(size, multisample, fullscreen, resizable, center, borderless, highDPI))
		{
			ErrorString("Failed to create form");
			return false;
		}
		if (!CreateContext(_window, multisample))
		{
			ErrorString("Context creation failed");
			return false;
		}

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
		if (!_window->SetSize(size, fullscreen, resizable, center, borderless, highDPI))
		{
			ErrorString("Failed to create form");
			return false;
		}
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
	/// Check freature support
	CheckFeatureSupport();

	return true;
}


bool Graphics::SetFullscreen(bool enable)
{

}

bool Graphics::SetMultisample(int multisample)
{

}

void Graphics::SetVSync(bool enable)
{

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

}

void Graphics::SetRenderTarget(Texture* renderTarget, Texture* depthStencil)
{

}

void Graphics::SetRenderTargets(const Vector<Texture*>& renderTargets, Texture* depthStencil)
{

}

void Graphics::SetViewport(const RectI& viewport)
{

}


void Graphics::SetVertexBuffer(size_t index, VertexBuffer* buffer)
{

}

void Graphics::SetIndexBuffer(IndexBuffer* buffer)
{

}

void Graphics::SetConstantBuffer(ShaderStage::Type stage, size_t index, ConstantBuffer* buffer)
{
}

void Graphics::SetTexture(size_t index, Texture* texture)
{
}

void Graphics::SetShaders(ShaderVariation* vs, ShaderVariation* ps)
{
}

void Graphics::SetColorState(const BlendModeDesc& blendMode, bool alphaToCoverage, unsigned char colorWriteMask)
{
	_renderState._blendMode = blendMode;
	_renderState._colorWriteMask = colorWriteMask;
	_renderState._alphaToCoverage = alphaToCoverage;

	_blendStateDirty = true;
}

void Graphics::SetColorState(BlendMode::Type blendMode, bool alphaToCoverage, unsigned char colorWriteMask)
{
	_renderState._blendMode = blendModes[blendMode];
	_renderState._colorWriteMask = colorWriteMask;
	_renderState._alphaToCoverage = alphaToCoverage;

	_blendStateDirty = true;
}

void Graphics::SetDepthState(CompareFunc::Type depthFunc, bool depthWrite, bool depthClip, int depthBias, float slopeScaledDepthBias)
{
	_renderState._depthFunc = depthFunc;
	_renderState._depthWrite = depthWrite;
	_renderState._depthClip = depthClip;
	_renderState._depthBias = depthBias;
	_renderState._slopeScaledDepthBias = slopeScaledDepthBias;

	_depthStateDirty = true;
	_rasterizerStateDirty = true;
}

void Graphics::SetRasterizerState(CullMode::Type cullMode, FillMode::Type fillMode)
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
			SetConstantBuffer((ShaderStage::Type)i, j, nullptr);
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

void Graphics::SetDepthState(CompareFunc::Type depthFunc, bool depthWrite, bool depthClip, int depthBias, float slopeScaledDepthBias)
{
	_renderState._depthFunc = depthFunc;
	_renderState._depthWrite = depthWrite;
	_renderState._depthClip = depthClip;
	_renderState._depthBias = depthBias;
	_renderState._slopeScaledDepthBias = slopeScaledDepthBias;

	_depthStateDirty = true;
	_rasterizerStateDirty = true;
}

void Graphics::SetRasterizerState(CullMode::Type cullMode, FillMode::Type fillMode)
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
			SetConstantBuffer((ShaderStage::Type)i, j, nullptr);
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
}

void Graphics::Draw(PrimitiveType::Type type, size_t vertexStart, size_t vertexCount)
{
}

void Graphics::DrawIndexed(PrimitiveType::Type type, size_t indexStart, size_t indexCount, size_t vertexStart)
{
}


void Graphics::DrawInstanced(PrimitiveType::Type type, size_t vertexStart, size_t vertexCount, size_t instanceStart, size_t instanceCount)
{
}

void Graphics::DrawIndexedInstanced(PrimitiveType::Type type, size_t indexStart, size_t indexCount, size_t vertexStart, size_t instanceStart, size_t instanceCount)
{
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

GraphicsContext* Graphics::RenderContext() const
{
	return _context;
}

Texture* Graphics::RenderTarget(size_t index) const
{
	return index < MAX_RENDERTARGETS ? _renderTargets[index] : nullptr;
}

VertexBuffer* Graphics::GetVertexBuffer(size_t index) const
{
	return index < MAX_VERTEX_STREAMS ? _vertexBuffers[index] : nullptr;
}

ConstantBuffer* Graphics::GetConstantBuffer(ShaderStage::Type stage, size_t index) const
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
}

void Graphics::CleanupFramebuffers(Texture* texture)
{
}

void Graphics::BindVBO(unsigned vbo)
{
}

void Graphics::BindUBO(unsigned ubo)
{
}

bool Graphics::CreateContext(Window* window, int multisample)
{
	// Create or recreate
	_context = new GraphicsContext(window);

	if (!_context->Create())
	{
		_context.Reset();
		return false;
	}

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
}

bool Graphics::PrepareDraw(bool instanced, size_t instanceStart)
{
}

void Graphics::ResetState()
{
	
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
#endif