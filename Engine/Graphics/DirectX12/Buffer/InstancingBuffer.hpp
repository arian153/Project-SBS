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
        Uint32                   GetInstanceCount() const;
        ComPtr<ID3D12Resource>   GetBuffer();
        D3D12_VERTEX_BUFFER_VIEW GetBufferView() const;

    private:
        ComPtr<ID3D12Resource>   m_buffer;
        D3D12_VERTEX_BUFFER_VIEW m_buffer_view;

        Uint32 m_max_count      = 0;
        std::vector<InstancingParams> m_data;
    };
}
