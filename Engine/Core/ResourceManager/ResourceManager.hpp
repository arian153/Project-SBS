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
    };
}
