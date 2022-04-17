/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

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

    ShaderArg ShaderResource::GetShaderPaths() const
    {
        return m_shader_path;
    }

    ShaderInfo ShaderResource::GetSubShaderInfo(Uint32 index) const
    {
        return m_shader_files[index].info;
    }

    ShaderArg ShaderResource::GetSubShaderEntry(Uint32 index) const
    {
        return m_shader_files[index].entry;
    }

    String ShaderResource::GetSubShaderName(Uint32 index) const
    {
        return m_shader_files[index].name;
    }

    Uint32 ShaderResource::GetSubShaderCount() const
    {
        return static_cast<Uint32>(m_shader_files.size());
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
                {
                    LoadInfo(path);
                    continue;
                }

                if (ext == ".hlsli" || ext == ".fx" || ext == ".hlsl")
                {
                    m_integrated_shader = path;
                    m_b_integrated = true;
                    continue;
                }

                m_b_integrated = false;

                if (ext == ".ps")
                    m_shader_path.pixel_shader = path;

                if (ext == ".vs")
                    m_shader_path.vertex_shader = path;

                if (ext == ".cs")
                    m_shader_path.compute_shader = path;

                if (ext == ".ds")
                    m_shader_path.domain_shader = path;

                if (ext == ".gs")
                    m_shader_path.geometry_shader = path;

                if (ext == ".hs")
                    m_shader_path.hull_shader = path;
            }

            m_has_sub_resource = true;
            m_b_loaded         = true;
        }
        else
        {
            m_has_sub_resource = false;
            m_b_loaded         = true;
        }
    }

    void ShaderResource::LoadInfo(const String& info_path)
    {
        if (info_path == "")
            return;

        Json::CharReaderBuilder builder;
        Json::CharReader*       reader    = builder.newCharReader();
        auto                    root_data = Json::Value();

        std::ifstream file(info_path, std::ifstream::binary);
        std::string   doc;
        std::getline(file, doc, static_cast<char>(EOF));

        if (reader->parse(doc.data(), doc.data() + doc.size(), &root_data, nullptr))
        {
            SubShaderFile sub_shader;

            if (!root_data["Name"].isNull() && root_data["Name"].isString())
            {
                sub_shader.name = root_data["Name"].asString();
            }
            else
            {
                sub_shader.name = std::filesystem::path(info_path).stem().generic_string();
            }

            if (!root_data["Layout"].isNull() && root_data["Layout"].isString())
            {
                sub_shader.info.input_layout = root_data["Layout"].asString();
            }

            if (!root_data["Shader Model"].isNull() && root_data["Shader Model"].isString())
            {
                sub_shader.info.shader_model = root_data["Shader Model"].asString();
            }

            if (!root_data["Rasterizer Type"].isNull())
            {
                if (root_data["Rasterizer Type"].isInt())
                {
                    sub_shader.info.rasterizer_type = static_cast<eRasterizerType>(root_data["Rasterizer Type"].asInt());
                }
            }

            if (!root_data["Render Target Type"].isNull())
            {
                if (root_data["Render Target Type"].isInt())
                {
                    sub_shader.info.render_target_type = static_cast<eRenderTargetType>(root_data["Render Target Type"].asInt());
                }
            }

            if (!root_data["Depth Stencil Type"].isNull())
            {
                if (root_data["Depth Stencil Type"].isInt())
                {
                    sub_shader.info.depth_stencil_type = static_cast<eDepthStencilType>(root_data["Depth Stencil Type"].asInt());
                }
            }

            if (!root_data["Blend Type"].isNull())
            {
                if (root_data["Blend Type"].isInt())
                {
                    sub_shader.info.blend_type = static_cast<eBlendType>(root_data["Blend Type"].asInt());
                }
            }

            if (!root_data["Topology Type"].isNull())
            {
                if (root_data["Topology Type"].isInt())
                {
                    sub_shader.info.topology = static_cast<eTopologyType>(root_data["Topology Type"].asInt());
                }
            }
            
            if (!root_data["Vertex Shader Entry"].isNull() && root_data["Vertex Shader Entry"].isString())
            {
                sub_shader.entry.vertex_shader = root_data["Vertex Shader Entry"].asString();
            }

            if (!root_data["Hull Shader Entry"].isNull() && root_data["Hull Shader Entry"].isString())
            {
                sub_shader.entry.hull_shader = root_data["Hull Shader Entry"].asString();
            }

            if (!root_data["Domain Shader Entry"].isNull() && root_data["Domain Shader Entry"].isString())
            {
                sub_shader.entry.domain_shader = root_data["Domain Shader Entry"].asString();
            }

            if (!root_data["Geometry Shader Entry"].isNull() && root_data["Geometry Shader Entry"].isString())
            {
                sub_shader.entry.geometry_shader = root_data["Geometry Shader Entry"].asString();
            }

            if (!root_data["Pixel Shader Entry"].isNull() && root_data["Pixel Shader Entry"].isString())
            {
                sub_shader.entry.pixel_shader = root_data["Pixel Shader Entry"].asString();
            }

            if (!root_data["Compute Shader Entry"].isNull() && root_data["Compute Shader Entry"].isString())
            {
                sub_shader.entry.compute_shader = root_data["Compute Shader Entry"].asString();
            }

            m_shader_files.push_back(sub_shader);
        }

        delete reader;
    }
}
