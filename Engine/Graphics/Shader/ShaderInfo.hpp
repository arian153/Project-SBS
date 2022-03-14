#pragma once
#include "../../EngineDefine.hpp"
#include "../../Utility/Utility.hpp"

namespace Engine
{
    enum class eShaderStage
    {
        VertexShader,
        HullShader,
        DomainShader,
        GeometryShader,
        PixelShader,
        ComputeShader
    };

    enum class eRasterizerType : Uint8
    {
        CullNone,
        CullFront,
        CullBack,
        Wireframe,
    };

    enum class eDepthStencilType : Uint8
    {
        Less,
        LessEqual,
        Greater,
        GreaterEqual,
        NoDepthTest,
        NoDepthTestNoWrite,
        LessNoWrite,
    };

    enum class eBlendType : Uint8
    {
        Default,
        AlphaBlend,
        OneToOneBlend,
        End,
    };

    enum class eTopologyType : Uint8
    {
        DotList,
        LineList,
        TriangleList
    };

    enum class eRenderTargetType : Uint8
    {
        Forward,
        DeferredGeo,
        DeferredLit
    };

    struct ShaderInfo
    {
        eRenderTargetType render_target_type = eRenderTargetType::Forward;
        eRasterizerType   rasterizer_type    = eRasterizerType::CullBack;
        eDepthStencilType depth_stencil_type = eDepthStencilType::Less;
        eBlendType        blend_type         = eBlendType::Default;
        eTopologyType     topology           = eTopologyType::TriangleList;
        String            shader_model       = "5_0";
        String            input_layout       = "DefaultLayout";
    };

    struct ShaderArg
    {
    public:
        String vertex_shader   = "";
        String hull_shader     = "";
        String domain_shader   = "";
        String geometry_shader = "";
        String pixel_shader    = "";
        String compute_shader  = "";
    };

    struct ShaderArgWide
    {
    public:
        void SetIntegrated(const StringWide& path)
        {
            vertex_shader   = path;
            hull_shader     = path;
            domain_shader   = path;
            geometry_shader = path;
            pixel_shader    = path;
            compute_shader  = path;
        }

        void SetIntegrated(const String& path)
        {
            StringWide integrated = ToWString(path);
            vertex_shader         = integrated;
            hull_shader           = integrated;
            domain_shader         = integrated;
            geometry_shader       = integrated;
            pixel_shader          = integrated;
            compute_shader        = integrated;
        }

        void Set(const ShaderArg& arg)
        {
            vertex_shader   = ToWString(arg.vertex_shader);
            hull_shader     = ToWString(arg.hull_shader);
            domain_shader   = ToWString(arg.domain_shader);
            geometry_shader = ToWString(arg.geometry_shader);
            pixel_shader    = ToWString(arg.pixel_shader);
            compute_shader  = ToWString(arg.compute_shader);
        }

    public:
        StringWide vertex_shader   = L"";
        StringWide hull_shader     = L"";
        StringWide domain_shader   = L"";
        StringWide geometry_shader = L"";
        StringWide pixel_shader    = L"";
        StringWide compute_shader  = L"";
    };
}
