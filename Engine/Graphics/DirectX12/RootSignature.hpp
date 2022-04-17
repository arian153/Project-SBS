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
    class RootSignature
    {
    public:
        RootSignature();
        ~RootSignature();

        void Initialize();

        ComPtr<ID3D12RootSignature> GetSignature();

    private:
        void CreateSamplerDesc();
        void CreateRootSignature();

    private:
        ComPtr<ID3D12RootSignature> m_signature;
        D3D12_STATIC_SAMPLER_DESC   m_sampler_desc = {};
    };
}
