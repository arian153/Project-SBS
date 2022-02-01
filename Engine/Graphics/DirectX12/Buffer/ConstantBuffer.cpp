#include "ConstantBuffer.hpp"

#include "../../GraphicsDefine.hpp"

namespace Engine
{
    ConstantBuffer::ConstantBuffer()
    {
    }

    ConstantBuffer::~ConstantBuffer()
    {
        Shutdown();
    }

    void ConstantBuffer::Init(eCBVRegister reg, Uint32 size, Uint32 count)
    {
        m_cbv_register = reg;

        m_element_size  = (size + 255) & ~255;
        m_element_count = count;

        CreateBuffer();
        CreateView();
    }

    void ConstantBuffer::Shutdown()
    {
        if (m_cbv_buffer)
        {
            if (m_cbv_buffer != nullptr)
                m_cbv_buffer->Unmap(0, nullptr);

            m_cbv_buffer = nullptr;
        }
    }

    void ConstantBuffer::Clear()
    {
        m_current_index = 0;
    }

    void ConstantBuffer::PushData(void* buffer, Uint32 size)
    {
        assert(m_current_index < m_element_count);
        assert(m_element_size == ((size + 255) & ~255));

        ::memcpy(&m_mapped_buffer[m_current_index * m_element_size], buffer, size);

        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = GetCpuHandle(m_current_index);
        g_core->GetRenderSystem()->GetTableDescriptorHeap()->SetCBV(cpu_handle, m_cbv_register);

        m_current_index++;
    }

    void ConstantBuffer::PushData(const void* buffer, Uint32 size)
    {
        assert(m_current_index < m_element_count);
        assert(m_element_size == ((size + 255) & ~255));

        ::memcpy(&m_mapped_buffer[m_current_index * m_element_size], buffer, size);

        D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = GetCpuHandle(m_current_index);
        DESCRIPTOR_HEAP->SetCBV(cpu_handle, m_cbv_register);

        m_current_index++;
    }

    D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(Uint32 index) const
    {
        D3D12_GPU_VIRTUAL_ADDRESS obj_cb_address = m_cbv_buffer->GetGPUVirtualAddress();
        obj_cb_address += static_cast<Uint64>(index) * static_cast<Uint64>(m_element_size);
        return obj_cb_address;
    }

    D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(Uint32 index) const
    {
        return CD3DX12_CPU_DESCRIPTOR_HANDLE(m_cpu_handle_begin, index * m_handle_increment_size);
    }

    void ConstantBuffer::CreateBuffer()
    {
        Uint32                buffer_size   = m_element_size * m_element_count;
        D3D12_HEAP_PROPERTIES heap_property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
        D3D12_RESOURCE_DESC   desc          = CD3DX12_RESOURCE_DESC::Buffer(buffer_size);

        DEVICE->CreateCommittedResource(
                                        &heap_property,
                                        D3D12_HEAP_FLAG_NONE,
                                        &desc,
                                        D3D12_RESOURCE_STATE_GENERIC_READ,
                                        nullptr,
                                        IID_PPV_ARGS(&m_cbv_buffer));

        m_cbv_buffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mapped_buffer));
        // We do not need to unmap until we are done with the resource.  However, we must not write to
        // the resource while it is in use by the GPU (so we must use synchronization techniques).
    }

    void ConstantBuffer::CreateView()
    {
        D3D12_DESCRIPTOR_HEAP_DESC descriptor_heap_desc = {};
        descriptor_heap_desc.NumDescriptors             = m_element_count;
        descriptor_heap_desc.Flags                      = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
        descriptor_heap_desc.Type                       = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
        DEVICE->CreateDescriptorHeap(&descriptor_heap_desc, IID_PPV_ARGS(&m_cbv_heap));

        m_cpu_handle_begin      = m_cbv_heap->GetCPUDescriptorHandleForHeapStart();
        m_handle_increment_size = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

        for (Uint32 i = 0; i < m_element_count; ++i)
        {
            D3D12_CPU_DESCRIPTOR_HANDLE cbv_handle = GetCpuHandle(i);

            D3D12_CONSTANT_BUFFER_VIEW_DESC cbv_desc = {};
            cbv_desc.BufferLocation                  = m_cbv_buffer->GetGPUVirtualAddress() + static_cast<Uint64>(m_element_size) * i;
            cbv_desc.SizeInBytes                     = m_element_size;   // CB size is required to be 256-byte aligned.

            DEVICE->CreateConstantBufferView(&cbv_desc, cbv_handle);
        }
    }
}
