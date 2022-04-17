/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include "DirectXIncludes.hpp"

namespace Engine
{
    class TableDescriptorHeap
    {
    public:
        void Init(Uint32 count);

        void Clear();
        void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE src_handle, eCBVRegister reg) const;
        void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE src_handle, eSRVRegister reg) const;

        void CommitTable();

        ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap();
        D3D12_CPU_DESCRIPTOR_HANDLE  GetCPUHandle(eCBVRegister reg) const;
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(eSRVRegister reg) const;


    private:
        D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(Uint32 reg) const;

    private:
        ComPtr<ID3D12DescriptorHeap> m_desc_heap;

        Uint64 m_handle_size         = 0;
        Uint64 m_group_size          = 0;
        Uint64 m_group_count         = 0;
        Uint32 m_current_group_index = 0;
    };
}
