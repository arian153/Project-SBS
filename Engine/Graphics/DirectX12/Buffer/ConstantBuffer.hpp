#pragma once
#include "../DirectXIncludes.hpp"

namespace Engine
{
    enum class eConstantBufferType : Uint32
    {
        GlobalPerFrame,
        Transform,
        Material,
        End
    };

    constexpr Uint32 CONSTANT_BUFFER_COUNT = static_cast<Uint32>(eConstantBufferType::End);

    class ConstantBuffer
    {
    public:
        ConstantBuffer();
        ~ConstantBuffer();

        void Init(eCBVRegister reg, Uint32 size, Uint32 count);
        void Shutdown();
        void Clear();
        void PushData(void* buffer, Uint32 size);
        void PushData(const void* buffer, Uint32 size);


        void SetGlobalData(void* buffer, Uint32 size) const;


        D3D12_GPU_VIRTUAL_ADDRESS   GetGpuVirtualAddress(Uint32 index) const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(Uint32 index) const;

    private:
        void CreateBuffer();
        void CreateView();

    private:
        ComPtr<ID3D12Resource>       m_cbv_buffer;
        ComPtr<ID3D12DescriptorHeap> m_cbv_heap;
        D3D12_CPU_DESCRIPTOR_HANDLE  m_cpu_handle_begin = {};

        BYTE*  m_mapped_buffer         = nullptr;
        Uint32 m_element_size          = 0;
        Uint32 m_element_count         = 0;
        Uint32 m_current_index         = 0;
        Uint32 m_handle_increment_size = 0;

        eCBVRegister m_cbv_register = {};
    };
}
