#pragma once

#include "../../AutoConfig.h"

#include "../../Container/Ptr.h"


namespace Auto3D
{

class Window;

struct ContextState
{
	static VkAllocationCallbacks*   g_Allocator = NULL;
	static VkInstance               g_Instance = VK_NULL_HANDLE;
	static VkPhysicalDevice         g_PhysicalDevice = VK_NULL_HANDLE;
	static VkDevice                 g_Device = VK_NULL_HANDLE;
	static uint32_t                 g_QueueFamily = (uint32_t)-1;
	static VkQueue                  g_Queue = VK_NULL_HANDLE;
	static VkDebugReportCallbackEXT g_DebugReport = VK_NULL_HANDLE;
	static VkPipelineCache          g_PipelineCache = VK_NULL_HANDLE;
	static VkDescriptorPool         g_DescriptorPool = VK_NULL_HANDLE;
};

/// OpenGL context associated with a _window, Win32 implementation.
class AUTO_API GraphicsContext : public RefCounted
{
public:
	/// Construct. Associate with a window, but do not create the context yet.
	GraphicsContext(Window* window);
	/// Destruct. Destroy the context if created.
	~GraphicsContext();

	/// Create context and initialize extensions. Return true on success. The pixel format can only be chosen once, so a context can not be created more than once to the same _window.
	bool Create();
	/// Present the backbuffer.
	void Present();
	/// Set vsync on/off.
	void SetVSync(bool enable);
	/// Return whether is initialized with a valid context.
	bool IsInitialized() const { return _contextHandle != nullptr; }
	/// Return null context,because context is OpenGL only.
	SDL_GLContext Context() { return NULL; }
private:
	/// Load global funcations. (only with vulkan)
	bool LoadGlobalFunctions();
	/// Create vulkan instance. (only with vulkan)
	void CreateInstance();
	/// Load instance functions. (only with vulkan)
	void LoadInstanceFunctions();
	/// Create vulkan surface. (only with vulkan)
	void CreateSurface();
	/// Check and find physical device. (only with vulkan)
	void FindPhysicalDevice();
	/// Create device. (only with vulkan)
	void CreateDevice();
	/// Load device functions. (only with vulkan)
	void LoadDeviceFunctions();
	/// Get queues. (only with vulkan)
	void GetQueues();
	/// Create semaphores. (only with vulkan)
	void CreateSemaphores();
	/// Create new swap chain and swap chain specific stuff.  (only with vulkan)
	void CreateNewSwapchainAndSwapchainSpecificStuff();

	/// Destroy the context.
	void Release();
	/// Vulkan context
	SDL_GLContext _contextHandle;
	/// Associated window.
	WeakPtr<Window> _window;

};

}