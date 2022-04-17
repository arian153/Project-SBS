/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "DirectX12Layer.hpp"

#include "../GraphicsDefine.hpp"
#include "../../Core/ApplicationSetting.hpp"
#include "../Data/Color.hpp"
#include "Buffer/ConstantBuffer.hpp"

namespace Engine
{
    DirectX12Layer::DirectX12Layer()
        : m_screen_viewport(), m_scissor_rect()
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
        RENDER_SYS->ResetRenderTargetGroups();

        // Resize the swap chain.

        m_swap_chain->ResizeBuffers(
                                    SWAP_CHAIN_BUFFER_COUNT,
                                    width, height,
                                    DXGI_FORMAT_R8G8B8A8_UNORM,
                                    DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);

        m_curr_back_buffer = 0;

        RENDER_SYS->CreateRenderTargetGroups();

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

        Uint32 back_buffer_index = GetBackBufferIndex();

        D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(

                                                                              RENDER_SYS->GetRTGroup(eRenderTargetGroupType::SwapChain)->GetRTTexture(back_buffer_index)->GetTex2D().Get(),
                                                                              D3D12_RESOURCE_STATE_PRESENT,
                                                                              D3D12_RESOURCE_STATE_RENDER_TARGET);

        m_command_list->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());

        RENDER_SYS->ClearConstantBuffers();

        m_command_list->ResourceBarrier(1, &barrier);

        // Set the viewport and scissor rect.  This needs to be reset whenever the command list is reset.
        m_command_list->RSSetViewports(1, &m_screen_viewport);
        m_command_list->RSSetScissorRects(1, &m_scissor_rect);
    }

    void DirectX12Layer::RenderEnd()
    {
        Uint32                 back_buffer_index = GetBackBufferIndex();
        D3D12_RESOURCE_BARRIER barrier           = CD3DX12_RESOURCE_BARRIER::Transition(
                                                                                        RENDER_SYS->GetRTGroup(eRenderTargetGroupType::SwapChain)->GetRTTexture(back_buffer_index)->GetTex2D().Get(),
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

    Uint32 DirectX12Layer::GetBackBufferIndex() const
    {
        return m_curr_back_buffer;
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
        swap_chain_desc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;
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
}
