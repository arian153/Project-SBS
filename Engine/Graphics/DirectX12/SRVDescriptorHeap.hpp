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
    class SRVDescriptorHeap
    {
    public:
        SRVDescriptorHeap();
        ~SRVDescriptorHeap();

        bool Init();

        ComPtr<ID3D12DescriptorHeap> GetHeap();

        void Increase();

        Uint32 GetNumDescriptor() const; 

    private:
        Uint32 m_num_descriptor = 1;
        ComPtr<ID3D12DescriptorHeap> m_srv_heap;
    };
}
