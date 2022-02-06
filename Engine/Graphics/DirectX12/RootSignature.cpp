#include "RootSignature.hpp"

#include "../GraphicsDefine.hpp"

namespace Engine
{
    RootSignature::RootSignature()
    {
    }

    RootSignature::~RootSignature()
    {
    }

    void RootSignature::Initialize()
    {
        CreateSamplerDesc();
        CreateRootSignature();
    }

    ComPtr<ID3D12RootSignature> RootSignature::GetSignature()
    {
        return m_signature;
    }

    void RootSignature::CreateSamplerDesc()
    {
        m_sampler_desc = CD3DX12_STATIC_SAMPLER_DESC(0);
    }

    void RootSignature::CreateRootSignature()
    {
        CD3DX12_DESCRIPTOR_RANGE ranges[] =
        {
            CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, CBV_REGISTER_COUNT - 1, 1), // b0 ~ b4
            CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, SRV_REGISTER_COUNT, 0), // t0 ~ t4
        };

        CD3DX12_ROOT_PARAMETER param[2];
        param[0].InitAsConstantBufferView(static_cast<Uint32>(eCBVRegister::b0));
        param[1].InitAsDescriptorTable(_countof(ranges), ranges);

        D3D12_ROOT_SIGNATURE_DESC signature_desc = CD3DX12_ROOT_SIGNATURE_DESC(_countof(param), param, 1, &m_sampler_desc);
        signature_desc.Flags                     = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

        ComPtr<ID3DBlob> blob_signature;
        ComPtr<ID3DBlob> blob_error;
        ::D3D12SerializeRootSignature(&signature_desc, D3D_ROOT_SIGNATURE_VERSION_1, &blob_signature, &blob_error);
        DEVICE->CreateRootSignature(
                                    0,
                                    blob_signature->GetBufferPointer(),
                                    blob_signature->GetBufferSize(),
                                    IID_PPV_ARGS(&m_signature));
    }
}
