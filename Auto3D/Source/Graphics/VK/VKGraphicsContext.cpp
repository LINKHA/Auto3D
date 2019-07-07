#include "../../AutoConfig.h"

#include "../../Container/WString.h"
#include "../../Debug/Log.h"
#include "VKGraphicsContext.h"
#include "../../Window/Window.h"

#include <cstring>
#include <vulkan/vulkan.h>

#include <SDL_vulkan.h>

#if defined(_WIN32 )| defined(_WIN64)
#	include <Windows.h>
#	include <wingdi.h>
#endif

#include "../../Debug/DebugNew.h"



namespace Auto3D
{

#define VULKAN_FUNCTIONS()                                              \
    VULKAN_DEVICE_FUNCTION(vkAcquireNextImageKHR)                       \
    VULKAN_DEVICE_FUNCTION(vkAllocateCommandBuffers)                    \
    VULKAN_DEVICE_FUNCTION(vkBeginCommandBuffer)                        \
    VULKAN_DEVICE_FUNCTION(vkCmdClearColorImage)                        \
    VULKAN_DEVICE_FUNCTION(vkCmdPipelineBarrier)                        \
    VULKAN_DEVICE_FUNCTION(vkCreateCommandPool)                         \
    VULKAN_DEVICE_FUNCTION(vkCreateFence)                               \
    VULKAN_DEVICE_FUNCTION(vkCreateImageView)                           \
    VULKAN_DEVICE_FUNCTION(vkCreateSemaphore)                           \
    VULKAN_DEVICE_FUNCTION(vkCreateSwapchainKHR)                        \
    VULKAN_DEVICE_FUNCTION(vkDestroyCommandPool)                        \
    VULKAN_DEVICE_FUNCTION(vkDestroyDevice)                             \
    VULKAN_DEVICE_FUNCTION(vkDestroyFence)                              \
    VULKAN_DEVICE_FUNCTION(vkDestroyImageView)                          \
    VULKAN_DEVICE_FUNCTION(vkDestroySemaphore)                          \
    VULKAN_DEVICE_FUNCTION(vkDestroySwapchainKHR)                       \
    VULKAN_DEVICE_FUNCTION(vkDeviceWaitIdle)                            \
    VULKAN_DEVICE_FUNCTION(vkEndCommandBuffer)                          \
    VULKAN_DEVICE_FUNCTION(vkFreeCommandBuffers)                        \
    VULKAN_DEVICE_FUNCTION(vkGetDeviceQueue)                            \
    VULKAN_DEVICE_FUNCTION(vkGetFenceStatus)                            \
    VULKAN_DEVICE_FUNCTION(vkGetSwapchainImagesKHR)                     \
    VULKAN_DEVICE_FUNCTION(vkQueuePresentKHR)                           \
    VULKAN_DEVICE_FUNCTION(vkQueueSubmit)                               \
    VULKAN_DEVICE_FUNCTION(vkResetCommandBuffer)                        \
    VULKAN_DEVICE_FUNCTION(vkResetFences)                               \
    VULKAN_DEVICE_FUNCTION(vkWaitForFences)                             \
    VULKAN_GLOBAL_FUNCTION(vkCreateInstance)                            \
    VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceExtensionProperties)      \
    VULKAN_GLOBAL_FUNCTION(vkEnumerateInstanceLayerProperties)          \
    VULKAN_INSTANCE_FUNCTION(vkCreateDevice)                            \
    VULKAN_INSTANCE_FUNCTION(vkDestroyInstance)                         \
    VULKAN_INSTANCE_FUNCTION(vkDestroySurfaceKHR)                       \
    VULKAN_INSTANCE_FUNCTION(vkEnumerateDeviceExtensionProperties)      \
    VULKAN_INSTANCE_FUNCTION(vkEnumeratePhysicalDevices)                \
    VULKAN_INSTANCE_FUNCTION(vkGetDeviceProcAddr)                       \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceFeatures)               \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceProperties)             \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceQueueFamilyProperties)  \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceCapabilitiesKHR) \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceFormatsKHR)      \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfacePresentModesKHR) \
    VULKAN_INSTANCE_FUNCTION(vkGetPhysicalDeviceSurfaceSupportKHR)

#define VULKAN_DEVICE_FUNCTION(name) static PFN_##name name = NULL;
#define VULKAN_GLOBAL_FUNCTION(name) static PFN_##name name = NULL;
#define VULKAN_INSTANCE_FUNCTION(name) static PFN_##name name = NULL;
	VULKAN_FUNCTIONS()
#undef VULKAN_DEVICE_FUNCTION
#undef VULKAN_GLOBAL_FUNCTION
#undef VULKAN_INSTANCE_FUNCTION


static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;

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
	
	return true;
}

void GraphicsContext::SetVSync(bool enable)
{
	
}

void GraphicsContext::Present()
{
	
}

bool GraphicsContext::LoadGlobalFunctions()
{
	vkGetInstanceProcAddr = SDL_Vulkan_GetVkGetInstanceProcAddr();
	if (!vkGetInstanceProcAddr)
	{
		ErrorStringF("SDL_Vulkan_GetVkGetInstanceProcAddr(): %s\n",SDL_GetError());
		return false;
	}

#define VULKAN_DEVICE_FUNCTION(name)
#define VULKAN_GLOBAL_FUNCTION(name)                                                   \
    name = (PFN_##name)vkGetInstanceProcAddr(VK_NULL_HANDLE, #name);                   \
    if(!name)                                                                          \
    {                                                                                  \
		ErrorStringF("vkGetInstanceProcAddr(VK_NULL_HANDLE, \"" #name "\") failed\n"); \
        return false;                                                                 \
    }
#define VULKAN_INSTANCE_FUNCTION(name)
	VULKAN_FUNCTIONS()
	return true;
#undef VULKAN_DEVICE_FUNCTION
#undef VULKAN_GLOBAL_FUNCTION
#undef VULKAN_INSTANCE_FUNCTION	
}

void GraphicsContext::CreateInstance()
{

}

void GraphicsContext::LoadInstanceFunctions()
{

}

void GraphicsContext::CreateSurface()
{

}

void GraphicsContext::FindPhysicalDevice()
{

}

void GraphicsContext::CreateDevice()
{

}

void GraphicsContext::LoadDeviceFunctions()
{

}

void GraphicsContext::GetQueues()
{

}

void GraphicsContext::CreateSemaphores()
{

}

void GraphicsContext::CreateNewSwapchainAndSwapchainSpecificStuff()
{

}

void GraphicsContext::Release()
{

}

}

