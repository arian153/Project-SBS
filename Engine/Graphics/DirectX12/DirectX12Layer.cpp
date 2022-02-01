#include "DirectX12Layer.hpp"

#include "../GraphicsDefine.hpp"
#include "../../Core/ApplicationSetting.hpp"
#include "../Data/Color.hpp"
#include "Buffer/ConstantBuffer.hpp"

namespace Engine
{
    DirectX12Layer::DirectX12Layer()
        : m_rtv_handle {}, m_screen_viewport(), m_scissor_rect()
    {
    }

    DirectX12Layer::~DirectX12Layer()
    {
        Shutdown();
    }

    void DirectX12Layer::Initialize(const WindowInfo& window_info)
    {
        CreateDevice();
        CreateCommandQueue();
        CreateSwapChain(window_info);
        CreateRTVAndDSVHeaps();

        OnResize(window_info.width, window_info.height, window_info.b_windowed);
        SetVSyncFlag(window_info.b_vsync);
    }

    void DirectX12Layer::Shutdown()
    {
        if (m_device != nullptr)
        {
            WaitSync();
            ::CloseHandle(m_fence_event);
        }
    }

    void DirectX12Layer::OnResize(Sint32 width, Sint32 height, bool b_windowed)
    {
        if (m_command_queue == nullptr)
            return;

        m_width  = width;
        m_height = height;

        // Flush before changing any resources.
        WaitSync();

        m_command_list->Reset(m_direct_cmd_list_alloc.Get(), nullptr);

        // Release the previous resources we will be recreating.
        for (Uint32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i)
        {
            m_rtv_buffer[i].Reset();
        }
        m_dsv_buffer.Reset();

        // Resize the swap chain.

        m_swap_chain->ResizeBuffers(
                                    SWAP_CHAIN_BUFFER_COUNT,
                                    width, height,
                                    m_rtv_format,
                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        m_curr_back_buffer = 0;

        // Create Render Target View
        CreateRTV();

        // Create the depth/stencil buffer and view.
        CreateDSV(width, height);

        // Transition the resource from its initial state to be used as a depth buffer.
        auto resource_barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                                                                     m_dsv_buffer.Get(),
                                                                     D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE);
        m_command_list->ResourceBarrier(1, &resource_barrier);

        // Execute the resize commands.
        m_command_list->Close();
        ID3D12CommandList* command_lists[] = { m_command_list.Get() };
        m_command_queue->ExecuteCommandLists(_countof(command_lists), command_lists);

        // Wait until resize is complete.
        WaitSync();

        // Update the viewport transform to cover the client area.
        m_screen_viewport = { 0, 0, static_cast<FLOAT>(width), static_cast<FLOAT>(height), 0.0f, 1.0f };
        m_scissor_rect    = CD3DX12_RECT(0, 0, width, height);
    }

    void DirectX12Layer::OnFullscreen(bool b_fullscreen) const
    {
        m_swap_chain->SetFullscreenState(b_fullscreen, nullptr);
        //OnResize(m_width, m_height, !b_fullscreen);
    }

    void DirectX12Layer::SetVSyncFlag(bool b_vsync_enabled)
    {
        m_sync_interval = b_vsync_enabled ? 1 : 0;
    }

    ComPtr<IDXGIFactory> DirectX12Layer::GetDXGI()
    {
        return m_dxgi_factory;
    }

    ComPtr<ID3D12Device> DirectX12Layer::GetDevice()
    {
        return m_device;
    }

    void DirectX12Layer::ClearCmdList() const
    {
    }

    void DirectX12Layer::ResizeCommands() const
    {
    }

    void DirectX12Layer::RenderBegin(const Color& clear_color) const
    {
        m_direct_cmd_list_alloc->Reset();
        //reset pso here
        m_command_list->Reset(m_direct_cmd_list_alloc.Get(), nullptr);

        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                                                                              CurrentBackRTVBuffer(),
                                                                              D3D12_RESOURCE_STATE_PRESENT,
                                                                              D3D12_RESOURCE_STATE_RENDER_TARGET);

        m_command_list->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

        m_command_list->ResourceBarrier(1, &barrier);

        // Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
        m_command_list->RSSetViewports(1, &m_screen_viewport);
        m_command_list->RSSetScissorRects(1, &m_scissor_rect);

        // Specify the buffers we are going to render to.
        D3D12_CPU_DESCRIPTOR_HANDLE back_buffer_view = GetBackRTVHandle();
        m_command_list->ClearRenderTargetView(back_buffer_view, clear_color.data, 0, nullptr);

        D3D12_CPU_DESCRIPTOR_HANDLE depth_stencil_view = GetDSVHandle();
        m_command_list->ClearDepthStencilView(depth_stencil_view, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

        m_command_list->OMSetRenderTargets(1, &back_buffer_view, FALSE, &depth_stencil_view);
    }

    void DirectX12Layer::RenderEnd()
    {
        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
                                                                              CurrentBackRTVBuffer(),
                                                                              D3D12_RESOURCE_STATE_RENDER_TARGET,
                                                                              D3D12_RESOURCE_STATE_PRESENT);

        m_command_list->ResourceBarrier(1, &barrier);
        m_command_list->Close();

        ID3D12CommandList* cmd_list_array[] = { m_command_list.Get() };
        m_command_queue->ExecuteCommandLists(_countof(cmd_list_array), cmd_list_array);

        m_swap_chain->Present(0, 0);

        WaitSync();
        SwapIndex();
    }

    ComPtr<ID3D12CommandQueue> DirectX12Layer::GetCmdQueue()
    {
        return m_command_queue;
    }

    ComPtr<ID3D12GraphicsCommandList> DirectX12Layer::GetCmdList()
    {
        return m_command_list;
    }

    ComPtr<ID3D12GraphicsCommandList> DirectX12Layer::GetResourceCmdList()
    {
        return m_resource_cmd_list;
    }

    DXGI_FORMAT DirectX12Layer::GetDSVFormat() const
    {
        return m_dsv_format;
    }

    DXGI_FORMAT DirectX12Layer::GetRTVFormat() const
    {
        return m_rtv_format;
    }

    void DirectX12Layer::CreateDevice()
    {
        #if defined(DEBUG) || defined(_DEBUG)
        {
            // Enable the D3D12 debug layer.
            D3D12GetDebugInterface(IID_PPV_ARGS(&m_debug_controller));
            m_debug_controller->EnableDebugLayer();
        }
        #endif

        CreateDXGIFactory1(IID_PPV_ARGS(&m_dxgi_factory));

        // Try to create hardware device.
        HRESULT hardware_result = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&m_device));

        // Fallback to WARP device.
        if (FAILED(hardware_result))
        {
            ComPtr<IDXGIAdapter> p_warp_adapter;
            m_dxgi_factory->EnumWarpAdapter(IID_PPV_ARGS(&p_warp_adapter));

            D3D12CreateDevice(p_warp_adapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&m_device));
        }

        m_rtv_descriptor_size         = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
        m_dsv_descriptor_size         = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
        m_cbv_srv_uav_descriptor_size = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
    }

    void DirectX12Layer::CreateCommandQueue()
    {
        D3D12_COMMAND_QUEUE_DESC queue_desc = {};
        queue_desc.Type                     = D3D12_COMMAND_LIST_TYPE_DIRECT;
        queue_desc.Flags                    = D3D12_COMMAND_QUEUE_FLAG_NONE;
        m_device->CreateCommandQueue(&queue_desc, IID_PPV_ARGS(&m_command_queue));

        m_device->CreateCommandAllocator(
                                         D3D12_COMMAND_LIST_TYPE_DIRECT,
                                         IID_PPV_ARGS(m_direct_cmd_list_alloc.GetAddressOf()));

        m_device->CreateCommandList(
                                    0, D3D12_COMMAND_LIST_TYPE_DIRECT,
                                    m_direct_cmd_list_alloc.Get(), nullptr,
                                    IID_PPV_ARGS(m_command_list.GetAddressOf()));

        m_command_list->Close();

        //resource command list
        m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_resource_cmd_list_alloc));
        m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_resource_cmd_list_alloc.Get(), nullptr, IID_PPV_ARGS(&m_resource_cmd_list));

        //fence
        m_device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
        m_fence_event = CreateEventEx(nullptr, nullptr, false, EVENT_ALL_ACCESS);
    }

    void DirectX12Layer::CreateSwapChain(const WindowInfo& window_info)
    {
        // Release the previous swap chain we will be recreating.
        m_swap_chain.Reset();

        DXGI_SWAP_CHAIN_DESC swap_chain_desc               = {};
        swap_chain_desc.BufferDesc.Width                   = static_cast<Uint32>(window_info.width);
        swap_chain_desc.BufferDesc.Height                  = static_cast<Uint32>(window_info.height);
        swap_chain_desc.BufferDesc.RefreshRate.Numerator   = 60;
        swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
        swap_chain_desc.BufferDesc.Format                  = m_rtv_format;
        swap_chain_desc.BufferDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        swap_chain_desc.BufferDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
        swap_chain_desc.SampleDesc.Count                   = 1;
        swap_chain_desc.SampleDesc.Quality                 = 0;
        swap_chain_desc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swap_chain_desc.BufferCount                        = SWAP_CHAIN_BUFFER_COUNT;
        swap_chain_desc.OutputWindow                       = window_info.hwnd;
        swap_chain_desc.Windowed                           = window_info.b_windowed;
        swap_chain_desc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;
        swap_chain_desc.Flags                              = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

        // Note: Swap chain uses queue to perform flush.
        m_dxgi_factory->CreateSwapChain(m_command_queue.Get(), &swap_chain_desc, m_swap_chain.GetAddressOf());
    }

    void DirectX12Layer::CreateRTVAndDSVHeaps()
    {
        D3D12_DESCRIPTOR_HEAP_DESC rtv_heap_desc = {};
        rtv_heap_desc.NumDescriptors             = SWAP_CHAIN_BUFFER_COUNT;
        rtv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
        rtv_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        rtv_heap_desc.NodeMask                   = 0;

        m_device->CreateDescriptorHeap(&rtv_heap_desc, IID_PPV_ARGS(m_rtv_heap.GetAddressOf()));

        D3D12_DESCRIPTOR_HEAP_DESC dsv_heap_desc = {};
        dsv_heap_desc.NumDescriptors             = 1;
        dsv_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
        dsv_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        dsv_heap_desc.NodeMask                   = 0;

        m_device->CreateDescriptorHeap(&dsv_heap_desc, IID_PPV_ARGS(m_dsv_heap.GetAddressOf()));
    }

    void DirectX12Layer::CreateRTV()
    {
        D3D12_CPU_DESCRIPTOR_HANDLE rtv_heap_begin = m_rtv_heap->GetCPUDescriptorHandleForHeapStart();

        for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
        {
            m_swap_chain->GetBuffer(i, IID_PPV_ARGS(&m_rtv_buffer[i]));
            m_rtv_handle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtv_heap_begin, i * m_rtv_descriptor_size);
            m_device->CreateRenderTargetView(m_rtv_buffer[i].Get(), nullptr, m_rtv_handle[i]);
        }
    }

    void DirectX12Layer::CreateDSV(Sint32 width, Sint32 height)
    {
        D3D12_HEAP_PROPERTIES heap_property      = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
        D3D12_RESOURCE_DESC   depth_stencil_desc = CD3DX12_RESOURCE_DESC::Tex2D(m_dsv_format, width, height, 1, 1);
        depth_stencil_desc.Flags                 = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
        D3D12_CLEAR_VALUE opt_clear              = CD3DX12_CLEAR_VALUE(m_dsv_format, 1.0f, 0);

        m_device->CreateCommittedResource(
                                          &heap_property,
                                          D3D12_HEAP_FLAG_NONE,
                                          &depth_stencil_desc,
                                          D3D12_RESOURCE_STATE_COMMON,
                                          &opt_clear,
                                          IID_PPV_ARGS(m_dsv_buffer.GetAddressOf()));

        // Create descriptor to mip level 0 of entire resource using the format of the resource.
        D3D12_DEPTH_STENCIL_VIEW_DESC dsv_desc = {};
        dsv_desc.Flags                         = D3D12_DSV_FLAG_NONE;
        dsv_desc.ViewDimension                 = D3D12_DSV_DIMENSION_TEXTURE2D;
        dsv_desc.Format                        = m_dsv_format;
        dsv_desc.Texture2D.MipSlice            = 0;

        m_dsv_handle = m_dsv_heap->GetCPUDescriptorHandleForHeapStart();
        m_device->CreateDepthStencilView(m_dsv_buffer.Get(), &dsv_desc, m_dsv_handle);
    }

    void DirectX12Layer::WaitSync()
    {
        // Advance the fence value to mark commands up to this fence point.
        m_current_fence++;

        // Add an instruction to the command queue to set a new fence point.  Because we 
        // are on the GPU timeline, the new fence point won't be set until the GPU finishes
        // processing all the commands prior to this Signal().
        m_command_queue->Signal(m_fence.Get(), m_current_fence);

        // Wait until the GPU has completed commands up to this fence point.
        if (m_fence->GetCompletedValue() < m_current_fence)
        {
            // Fire event when GPU hits current fence.  
            m_fence->SetEventOnCompletion(m_current_fence, m_fence_event);

            // Wait until the GPU hits current fence event is fired.
            WaitForSingleObject(m_fence_event, INFINITE);
        }
    }

    void DirectX12Layer::FlushResourceCommandQueue()
    {
        m_resource_cmd_list->Close();

        ID3D12CommandList* cmd_list_arr[] = { m_resource_cmd_list.Get() };
        m_command_queue->ExecuteCommandLists(_countof(cmd_list_arr), cmd_list_arr);

        WaitSync();

        m_resource_cmd_list_alloc->Reset();
        m_resource_cmd_list->Reset(m_resource_cmd_list_alloc.Get(), nullptr);
    }

    void DirectX12Layer::Present() const
    {
        m_swap_chain->Present(m_sync_interval, 0);
    }

    void DirectX12Layer::SwapIndex()
    {
        m_curr_back_buffer = (m_curr_back_buffer + 1) % SWAP_CHAIN_BUFFER_COUNT;
    }

    ID3D12Resource* DirectX12Layer::CurrentBackRTVBuffer() const
    {
        return m_rtv_buffer[m_curr_back_buffer].Get();
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Layer::CurrentBackRTVHandle() const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(
                                             m_rtv_heap->GetCPUDescriptorHandleForHeapStart(),
                                             m_curr_back_buffer,
                                             m_rtv_descriptor_size);
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Layer::GetDSVHandle() const
    {
        return m_dsv_handle;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Layer::GetRTVHandle(Sint32 idx) const
    {
        return m_rtv_handle[idx];
    }

    D3D12_CPU_DESCRIPTOR_HANDLE DirectX12Layer::GetBackRTVHandle() const
    {
        return m_rtv_handle[m_curr_back_buffer];
    }
}
