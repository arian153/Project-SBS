#include "ShaderProgram.hpp"

#include "../GraphicsDefine.hpp"
#include "../RenderSystem.hpp"
#include "../DirectX12/InputLayout.hpp"

namespace Engine
{
    ShaderProgram::ShaderProgram()
    {
    }

    ShaderProgram::~ShaderProgram()
    {
    }

    void ShaderProgram::Initialize(const StringWide& integrated_path, const ShaderArg& shader_entry_point, const ShaderInfo& shader_info)
    {
        m_shader_info  = shader_info;
        m_entry_points = shader_entry_point;
        m_shader_paths.SetIntegrated(integrated_path);
    }

    void ShaderProgram::Initialize(const ShaderArg& shader_path, const ShaderArg& shader_entry_point, const ShaderInfo& shader_info)
    {
        m_shader_info  = shader_info;
        m_entry_points = shader_entry_point;
        m_shader_paths.Set(shader_path);
    }

    void ShaderProgram::Bind() const
    {
        CMD_LIST->SetPipelineState(m_pipeline_state.Get());
    }

    String ShaderProgram::GetName() const
    {
        return m_shader_name;
    }

    bool ShaderProgram::CreateShader(const StringWide& path, const String& name, const String& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shader_byte_code)
    {
        Uint32 compile_flag;
        #ifdef _DEBUG
        compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
        #else
        compile_flag = 0;
        #endif

        HRESULT result = ::D3DCompileFromFile(
                                              path.c_str(),
                                              nullptr,
                                              D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                              name.c_str(),
                                              version.c_str(),
                                              compile_flag,
                                              0,
                                              &m_temp_blob,
                                              &m_err_blob);

        if (FAILED(result))
        {
            #ifdef _DEBUG

            String msg = "Failed Shader Compilation : " + ToString(path) + "\n";

            // Get a pointer to the error message text buffer.
            auto compile_errors = static_cast<char*>(m_err_blob->GetBufferPointer());
            // Get the length of the message.
            size_t buffer_size = m_err_blob->GetBufferSize();
            // Write out the error message.
            std::string error_message(compile_errors, buffer_size);

            msg += error_message;

            ::MessageBoxA(nullptr, msg.c_str(), nullptr, MB_OK);
            #endif
            return false;
        }

        blob = m_temp_blob;

        shader_byte_code = { blob->GetBufferPointer(), blob->GetBufferSize() };
        return true;
    }

    bool ShaderProgram::CreateVertexShader(const StringWide& path, const String& name, const String& version)
    {
        return CreateShader(path, name, version, m_vs_blob, m_pipeline_desc.VS);
    }

    bool ShaderProgram::CreatePixelShader(const StringWide& path, const String& name, const String& version)
    {
        return CreateShader(path, name, version, m_ps_blob, m_pipeline_desc.PS);
    }

    bool ShaderProgram::Compile()
    {
        if (CreateVertexShader(m_shader_paths.vertex_shader, m_entry_points.vertex_shader, "vs_" + m_shader_info.shader_model) == false)
            return false;

        if (CreatePixelShader(m_shader_paths.pixel_shader, m_entry_points.pixel_shader, "ps_" + m_shader_info.shader_model) == false)
            return false;

        std::vector<D3D12_INPUT_ELEMENT_DESC> input_layout_desc;
        SHADER_MANAGER->GetInputLayout(m_shader_info.input_layout).GetLayoutDesc(input_layout_desc);

        m_pipeline_desc.InputLayout           = { input_layout_desc.data(), static_cast<UINT>(input_layout_desc.size()) };
        m_pipeline_desc.pRootSignature        = ROOT_SIGNATURE.Get();
        m_pipeline_desc.RasterizerState       = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
        m_pipeline_desc.BlendState            = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
        m_pipeline_desc.DepthStencilState     = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
        m_pipeline_desc.SampleMask            = UINT_MAX;
        m_pipeline_desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
        m_pipeline_desc.NumRenderTargets      = 1;
        m_pipeline_desc.RTVFormats[0]         = RENDER_SYS_DX12->GetRTVFormat();
        m_pipeline_desc.SampleDesc.Count      = 1;
        m_pipeline_desc.DSVFormat             = RENDER_SYS_DX12->GetDSVFormat();

        switch (m_shader_info.rasterizer_type)
        {
        case eRasterizerType::CullNone:
            m_pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            m_pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
            break;
        case eRasterizerType::CullFront:
            m_pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            m_pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_FRONT;
            break;
        case eRasterizerType::CullBack:
            m_pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
            m_pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
            break;
        case eRasterizerType::Wireframe:
            m_pipeline_desc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
            m_pipeline_desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
            break;
        default:
            break;
        }

        switch (m_shader_info.depth_stencil_type)
        {
        case eDepthStencilType::Less:
            m_pipeline_desc.DepthStencilState.DepthEnable = TRUE;
            m_pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
            break;
        case eDepthStencilType::LessEqual:
            m_pipeline_desc.DepthStencilState.DepthEnable = TRUE;
            m_pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
            break;
        case eDepthStencilType::Greater:
            m_pipeline_desc.DepthStencilState.DepthEnable = TRUE;
            m_pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;
            break;
        case eDepthStencilType::GreaterEqual:
            m_pipeline_desc.DepthStencilState.DepthEnable = TRUE;
            m_pipeline_desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_GREATER_EQUAL;
            break;
        case eDepthStencilType::NoDepthTest:
            break;
        case eDepthStencilType::NoDepthTestNoWrite:
            break;
        case eDepthStencilType::LessNoWrite:
            break;
        default:
            break;
        }

        HRESULT result = DEVICE->CreateGraphicsPipelineState(&m_pipeline_desc, IID_PPV_ARGS(&m_pipeline_state));
        if (FAILED(result))
        {
            #ifdef _DEBUG
            ::MessageBoxA(nullptr, "Failed Pipeline State Creation!", nullptr, MB_OK);
            #endif
            return false;
        }

        return true;
    }
}
