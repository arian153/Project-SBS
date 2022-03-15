#pragma once
#include "../../../EngineDefine.hpp"
#include "../../../Math/Algebra/Matrix44.hpp"
#include "../../Data/Color.hpp"
#include "../DirectXIncludes.hpp"

namespace Engine
{
    struct InstancingParams
    {
        Matrix44 mat_world;
        Matrix44 mat_wv;
        Matrix44 mat_wvp;
        Color    diffuse;
    };

    class InstancingBuffer
    {
    public:
        InstancingBuffer();
        ~InstancingBuffer();
        void Init(Uint32 max_count = 10);

        void Clear();
        void AddData(const InstancingParams& params);
        void Bind();

    public:
        Uint32                   GetCount() const;
        ComPtr<ID3D12Resource>   GetBuffer();
        D3D12_VERTEX_BUFFER_VIEW GetBufferView() const;

    private:
        ComPtr<ID3D12Resource>   m_buffer;
        D3D12_VERTEX_BUFFER_VIEW m_buffer_view;

        Uint32                        m_max_count = 0;
        std::vector<InstancingParams> m_data;
    };
}
