#pragma once
#include "DirectXIncludes.hpp"

namespace Engine
{
    class TableDescriptorHeap;
    class Color;
    struct WindowInfo;

    class DirectX12Layer
    {
    public:
        DirectX12Layer();
        ~DirectX12Layer();

        void Initialize(const WindowInfo& window_info);
        void Shutdown();

        void OnResize(Sint32 width, Sint32 height, bool b_windowed);
        void OnFullscreen(bool b_fullscreen) const;

        void SetVSyncFlag(bool b_vsync_enabled);

        ComPtr<IDXGIFactory> GetDXGI();
        ComPtr<ID3D12Device> GetDevice();

        void ClearCmdList() const;
        void ResizeCommands() const;

        void RenderBegin(const Color& clear_color) const;
        void RenderEnd();
        void WaitSync();

        void FlushResourceCommandQueue();

        ComPtr<ID3D12CommandQueue>        GetCmdQueue();
        ComPtr<ID3D12GraphicsCommandList> GetCmdList();
        ComPtr<ID3D12GraphicsCommandList> GetResourceCmdList();

        DXGI_FORMAT GetDSVFormat() const;
        DXGI_FORMAT GetRTVFormat() const;

    private:
        void CreateDevice();

        void CreateCommandQueue();
        void CreateSwapChain(const WindowInfo& window_info);
        void CreateRTVAndDSVHeaps();

        void CreateRTV();
        void CreateDSV(Sint32 width, Sint32 height);

        void Present() const;
        void SwapIndex();

        ID3D12Resource*             CurrentBackRTVBuffer() const;
        D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackRTVHandle() const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetDSVHandle() const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetRTVHandle(Sint32 idx) const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTVHandle() const;

    private:
        friend class RenderSystem;

    private:
        //device
        ComPtr<IDXGIFactory4> m_dxgi_factory;
        ComPtr<ID3D12Device>  m_device;
        ComPtr<ID3D12Debug>   m_debug_controller;

        //fence
        ComPtr<ID3D12Fence> m_fence;
        HANDLE              m_fence_event   = INVALID_HANDLE_VALUE;
        UINT64              m_current_fence = 0;
        //command queue
        ComPtr<ID3D12CommandQueue>        m_command_queue;
        ComPtr<ID3D12CommandAllocator>    m_direct_cmd_list_alloc;
        ComPtr<ID3D12GraphicsCommandList> m_command_list;
        ComPtr<ID3D12CommandAllocator>    m_resource_cmd_list_alloc;
        ComPtr<ID3D12GraphicsCommandList> m_resource_cmd_list;

        //swap chain
        ComPtr<IDXGISwapChain> m_swap_chain;

        //Render Target View
        ComPtr<ID3D12Resource>       m_rtv_buffer[SWAP_CHAIN_BUFFER_COUNT];
        ComPtr<ID3D12DescriptorHeap> m_rtv_heap;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_rtv_handle[SWAP_CHAIN_BUFFER_COUNT];
        DXGI_FORMAT                  m_rtv_format = DXGI_FORMAT_R8G8B8A8_UNORM;

        // Depth Stencil View
        ComPtr<ID3D12Resource>       m_dsv_buffer;
        ComPtr<ID3D12DescriptorHeap> m_dsv_heap;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_dsv_handle = {};
        DXGI_FORMAT                  m_dsv_format = DXGI_FORMAT_D32_FLOAT;

        //viewport
        D3D12_VIEWPORT m_screen_viewport;
        D3D12_RECT     m_scissor_rect;
        Sint32          m_width  = 0;
        Sint32          m_height = 0;

        Uint32 m_rtv_descriptor_size         = 0;
        Uint32 m_dsv_descriptor_size         = 0;
        Uint32 m_cbv_srv_uav_descriptor_size = 0;
        Uint32 m_curr_back_buffer            = 0;
        Uint32 m_sync_interval               = 0;

        D3D_DRIVER_TYPE m_d3d_driver_type = D3D_DRIVER_TYPE_HARDWARE;
    };
}
