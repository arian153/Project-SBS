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
#include "ShaderInfo.hpp"
#include "../DirectX12/DirectXIncludes.hpp"

#include "../../EngineDefine.hpp"

namespace Engine
{
    class ShaderProgram
    {
    public:
        explicit ShaderProgram();
        ~ShaderProgram();

        void Initialize(const StringWide& integrated_path, const ShaderArg& shader_entry_point, const ShaderInfo& shader_info);
        void Initialize(const ShaderArg& shader_path, const ShaderArg& shader_entry_point, const ShaderInfo& shader_info);
        void Bind() const;

        String GetName() const;
        bool IsDeferred() const;

    private:
        bool CreateShader(const StringWide& path, const String& name, const String& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shader_byte_code);
        bool CreateVertexShader(const StringWide& path, const String& name, const String& version);
        bool CreatePixelShader(const StringWide& path, const String& name, const String& version);
        bool Compile();

    private:
        friend class ShaderManager;

    private:
        ComPtr<ID3DBlob> m_vs_blob;
        ComPtr<ID3DBlob> m_ps_blob;
        ComPtr<ID3DBlob> m_err_blob;
        ComPtr<ID3DBlob> m_temp_blob;

        ComPtr<ID3D12PipelineState>        m_pipeline_state;
        D3D12_GRAPHICS_PIPELINE_STATE_DESC m_pipeline_desc = {};

        ShaderArg     m_entry_points;
        ShaderArgWide m_shader_paths;
        ShaderInfo    m_shader_info;
        String        m_shader_name = "";
    };
}
