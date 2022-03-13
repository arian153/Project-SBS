#include "ShaderResource.hpp"

#include <filesystem>
#include <fstream>

#include "../../../External/JSONCPP/json/json.h"
#include "../../../Utility/FileUtility.hpp"
#include "../../../Utility/Utility.hpp"

namespace Engine
{
    ShaderResource::ShaderResource(const String& path)
        : Resource(path)
    {
        m_type = eResourceType::Shader;
    }

    ShaderResource::~ShaderResource()
    {
    }

    bool ShaderResource::HasSubResource() const
    {
        return m_has_sub_resource;
    }

    bool ShaderResource::IsIntegrated() const
    {
        return m_b_integrated;
    }

    bool ShaderResource::IsHeader() const
    {
        return m_b_header;
    }

    StringWide ShaderResource::GetShaderPath() const
    {
        if (m_has_sub_resource)
            return ToWString(m_integrated_shader);

        return m_wide_path;
    }

    StringWide ShaderResource::GetShaderPath(eShaderStage stage) const
    {
        if (m_b_integrated)
            return GetShaderPath();

        if (m_has_sub_resource)
        {
            switch (stage)
            {
            case eShaderStage::VertexShader:
                return ToWString(m_shader_path.vertex_shader);
            case eShaderStage::HullShader:
                return ToWString(m_shader_path.hull_shader);
            case eShaderStage::DomainShader:
                return ToWString(m_shader_path.domain_shader);
            case eShaderStage::GeometryShader:
                return ToWString(m_shader_path.geometry_shader);
            case eShaderStage::PixelShader:
                return ToWString(m_shader_path.pixel_shader);
            case eShaderStage::ComputeShader:
                return ToWString(m_shader_path.compute_shader);
            default:
                break;
            }
        }

        return m_wide_path;
    }

    ShaderInfo ShaderResource::GetShaderInfo() const
    {
        return m_shader_info;
    }

    ShaderArg ShaderResource::GetShaderEntry() const
    {
        return m_shader_entry;
    }

    ShaderArg ShaderResource::GetShaderPaths() const
    {
        return m_shader_path;
    }

    void ShaderResource::Initialize()
    {
        Load();
    }

    void ShaderResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    void ShaderResource::Load()
    {
        if (FileUtility::IsDirectory(m_file_path))
        {
            for (auto& iterated_path : std::filesystem::directory_iterator(m_file_path))
            {
                String ext  = iterated_path.path().extension().generic_string();
                String path = iterated_path.path().generic_string();

                if (ext == ".info")
                    m_info_path = path;

                if (ext == ".hlsli")
                    m_integrated_shader = path;

                m_sub_resources.emplace_back(path);
            }

            m_has_sub_resource = true;
            m_b_loaded         = true;
            LoadInfo();
        }
        else
        {
            m_has_sub_resource = false;
            m_b_loaded         = true;
        }
    }

    void ShaderResource::LoadInfo()
    {
        if (m_info_path == "")
            return;

        Json::CharReaderBuilder builder;
        Json::CharReader*       reader    = builder.newCharReader();
        auto                    root_data = Json::Value();

        std::ifstream file(m_info_path, std::ifstream::binary);
        std::string   doc;
        std::getline(file, doc, static_cast<char>(EOF));
        if (reader->parse(doc.data(), doc.data() + doc.size(), &root_data, nullptr))
        {
            if (!root_data["Layout"].isNull() && root_data["Layout"].isString())
            {
                m_shader_info.input_layout = root_data["Layout"].asString();
            }

            if (!root_data["Shader Model"].isNull() && root_data["Shader Model"].isString())
            {
                m_shader_info.shader_model = root_data["Shader Model"].asString();
            }

            if (!root_data["Rasterizer Type"].isNull())
            {
                if (root_data["Rasterizer Type"].isInt())
                {
                    m_shader_info.rasterizer_type = static_cast<eRasterizerType>(root_data["Rasterizer Type"].asInt());
                }
            }

            if (!root_data["Render Target Type"].isNull())
            {
                if (root_data["Render Target Type"].isInt())
                {
                    m_shader_info.render_target_type = static_cast<eRenderTargetType>(root_data["Render Target Type"].asInt());
                }
            }

           
            if (!root_data["Depth Stencil Type"].isNull())
            {
                if (root_data["Depth Stencil Type"].isInt())
                {
                    m_shader_info.depth_stencil_type = static_cast<eDepthStencilType>(root_data["Depth Stencil Type"].asInt());
                }
            }

            if (!root_data["Blend Type"].isNull())
            {
                if (root_data["Blend Type"].isInt())
                {
                    m_shader_info.blend_type = static_cast<eBlendType>(root_data["Blend Type"].asInt());
                }
            }

            if (!root_data["Topology Type"].isNull())
            {
                if (root_data["Topology Type"].isInt())
                {
                    m_shader_info.topology = static_cast<eTopologyType>(root_data["Topology Type"].asInt());
                }
            }

            if (!root_data["Is Integrated"].isNull() && root_data["Is Integrated"].isBool())
            {
                m_b_integrated = root_data["Is Integrated"].asBool();
            }

            if (!root_data["Vertex Shader Entry"].isNull() && root_data["Vertex Shader Entry"].isString())
            {
                m_shader_entry.vertex_shader = root_data["Vertex Shader Entry"].asString();
            }

            if (!root_data["Hull Shader Entry"].isNull() && root_data["Hull Shader Entry"].isString())
            {
                m_shader_entry.hull_shader = root_data["Hull Shader Entry"].asString();
            }

            if (!root_data["Domain Shader Entry"].isNull() && root_data["Domain Shader Entry"].isString())
            {
                m_shader_entry.domain_shader = root_data["Domain Shader Entry"].asString();
            }

            if (!root_data["Geometry Shader Entry"].isNull() && root_data["Geometry Shader Entry"].isString())
            {
                m_shader_entry.geometry_shader = root_data["Geometry Shader Entry"].asString();
            }

            if (!root_data["Pixel Shader Entry"].isNull() && root_data["Pixel Shader Entry"].isString())
            {
                m_shader_entry.pixel_shader = root_data["Pixel Shader Entry"].asString();
            }

            if (!root_data["Compute Shader Entry"].isNull() && root_data["Compute Shader Entry"].isString())
            {
                m_shader_entry.compute_shader = root_data["Compute Shader Entry"].asString();
            }

            if (!root_data["Vertex Shader Path"].isNull() && root_data["Vertex Shader Path"].isString())
            {
                m_shader_path.vertex_shader = root_data["Vertex Shader Path"].asString();
            }

            if (!root_data["Hull Shader Path"].isNull() && root_data["Hull Shader Path"].isString())
            {
                m_shader_path.hull_shader = root_data["Hull Shader Path"].asString();
            }

            if (!root_data["Domain Shader Path"].isNull() && root_data["Domain Shader Path"].isString())
            {
                m_shader_path.domain_shader = root_data["Domain Shader Path"].asString();
            }

            if (!root_data["Geometry Shader Path"].isNull() && root_data["Geometry Shader Path"].isString())
            {
                m_shader_path.geometry_shader = root_data["Geometry Shader Path"].asString();
            }

            if (!root_data["Pixel Shader Path"].isNull() && root_data["Pixel Shader Path"].isString())
            {
                m_shader_path.pixel_shader = root_data["Pixel Shader Path"].asString();
            }

            if (!root_data["Compute Shader Path"].isNull() && root_data["Compute Shader Path"].isString())
            {
                m_shader_path.compute_shader = root_data["Compute Shader Path"].asString();
            }
        }

        delete reader;
    }
}
