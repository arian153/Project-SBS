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
        ShaderArg  GetShaderPaths() const;

        ShaderInfo GetSubShaderInfo(Uint32 index) const;
        ShaderArg  GetSubShaderEntry(Uint32 index) const;
        String     GetSubShaderName(Uint32 index) const;

        Uint32 GetSubShaderCount() const;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();
        void LoadInfo(const String& info_path);

    private:
        friend class ResourceManager;

    private:
        bool m_has_sub_resource = false;
        bool m_b_integrated     = true;
        bool m_b_header         = false;

        String    m_integrated_shader = "";
        ShaderArg m_shader_path;

        struct SubShaderFile
        {
            String     name;
            ShaderInfo info;
            ShaderArg  entry;
        };

        std::vector<SubShaderFile> m_shader_files;
    };
}
