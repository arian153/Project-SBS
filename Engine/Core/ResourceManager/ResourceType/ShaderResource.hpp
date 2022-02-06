#pragma once
#include "../Resource.hpp"
#include "../../../Graphics/Shader/ShaderInfo.hpp"

namespace Engine
{
    class ShaderResource final : public Resource
    {
    public:
        explicit ShaderResource(const String& path);
        ~ShaderResource() override;

        bool       HasSubResource() const;
        bool       IsIntegrated() const;
        bool       IsHeader() const;
        StringWide GetShaderPath() const;
        StringWide GetShaderPath(eShaderStage stage) const;

        ShaderInfo GetShaderInfo() const;
        ShaderArg  GetShaderEntry() const;
        ShaderArg  GetShaderPaths() const;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();
        void LoadInfo();

    private:
        friend class ResourceManager;

    private:
        bool m_has_sub_resource = false;
        bool m_b_integrated     = true;
        bool m_b_header         = false;

        String m_integrated_shader = "";
        String m_info_path         = "";

        ShaderInfo m_shader_info;
        ShaderArg  m_shader_entry;
        ShaderArg  m_shader_path;

        std::vector<String> m_sub_resources;
    };
}
