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
