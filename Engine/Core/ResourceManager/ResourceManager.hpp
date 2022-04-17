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

#include "../../System.hpp"
#include "ResourceType/ModelResource.hpp"
#include "ResourceType/ShaderResource.hpp"
#include "ResourceType/TextureResource.hpp"

namespace Engine
{
    class ResourceManager final : public System
    {
    public:
        ResourceManager();
        ~ResourceManager() override;

        void ReloadPath(const String& path);
        void ReloadResource(Resource* resource);

        void GetModelResources(std::vector<ModelResource*>& model_resources);
        void GetShaderResources(std::vector<ShaderResource*>& shader_resources);
        void GetTextureResources(std::vector<TextureResource*>& texture_resources);

        ShaderResource* GetShaderResourcePath(const String& file_path);
        ShaderResource* GetShaderResourceName(const String& file_name);

        TextureResource* GetTextureResourcePath(const String& file_path);
        TextureResource* GetTextureResourceName(const String& file_name);

        ModelResource* GetModelResourcePath(const String& file_path);
        ModelResource* GetModelResourceName(const String& file_name);

        SPtr<Texture> GetCreatedTexture(const String& name);

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        void SetRootPath(const String& path);
        void QueryFilePathRecursive(const String& path, std::vector<String>& file_list, std::vector<String>& directory_list) const;

        void BuildResource();
        void AddResource(const String& path);

    private:
        friend class CoreSystem;

    private:
        String              m_root_path = "";
        std::vector<String> m_file_list;
        std::vector<String> m_directory_list;

        HashMap<String, ModelResource>   m_model_map;
        HashMap<String, ShaderResource>  m_shader_map;
        HashMap<String, TextureResource> m_texture_map;

        HashMap<String, SPtr<Texture>> m_created_texture_map;
    };
}
