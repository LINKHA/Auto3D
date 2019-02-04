#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <shellapi.h> // For CommandLineToArgvW

// Windows Runtime Library. Needed for Microsoft::WRL::ComPtr<> template class.
#include <wrl.h>
using namespace Microsoft::WRL;

// DirectX 12 specific headers.
#include <d3d12.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// D3D12 extension library.
#include <d3dx12.h>

// STL Headers
#include <algorithm>
#include <cassert>
#include <chrono>


namespace Auto3D {

inline std::string HrToString(HRESULT hr)
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}

class HrException : public std::runtime_error
{
public:
	HrException(HRESULT hr) : std::runtime_error(HrToString(hr)), m_hr(hr) {}
	HRESULT Error() const { return m_hr; }
private:
	const HRESULT m_hr;
};


inline void ThrowIfFailed(HRESULT hr)
{
	if (FAILED(hr))
	{
		throw HrException(hr);
	}
}

ComPtr<IDXGIAdapter4> DxGetAdapter(bool useWarp);

ComPtr<ID3D12Device2> DxCreateDevice(ComPtr<IDXGIAdapter4> adapter);

ComPtr<ID3D12CommandQueue> DxCreateCommandQueue(ComPtr<ID3D12Device2> device, D3D12_COMMAND_LIST_TYPE type);

ComPtr<ID3D12DescriptorHeap> DxCreateDescriptorHeap(ComPtr<ID3D12Device2> device,D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors);

ComPtr<ID3D12CommandAllocator> DxCreateCommandAllocator(ComPtr<ID3D12Device2> device,
	D3D12_COMMAND_LIST_TYPE type);

ComPtr<ID3D12GraphicsCommandList> DxCreateCommandList(ComPtr<ID3D12Device2> device,
	ComPtr<ID3D12CommandAllocator> commandAllocator, D3D12_COMMAND_LIST_TYPE type);

ComPtr<ID3D12Fence> DxCreateFence(ComPtr<ID3D12Device2> device);

HANDLE DxCreateEventHandle();

}