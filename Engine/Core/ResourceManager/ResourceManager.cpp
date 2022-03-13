#include "ResourceManager.hpp"

#include <algorithm>
#include <memory>

#include "Resource.hpp"
#include "../../Graphics/DirectX12/Texture.hpp"
#include "../../Utility/FileUtility.hpp"

namespace Engine
{
    bool CompareResource(Resource* a, Resource* b)
    {
        return std::strcmp(a->FileName().c_str(), b->FileName().c_str()) < 0;
    }

    ResourceManager::ResourceManager()
    {
    }

    ResourceManager::~ResourceManager()
    {
    }

    void ResourceManager::ReloadPath(const String& path)
    {
        {
            auto found = m_model_map.find(path);
            if (found != m_model_map.end())
            {
                found->second.Load();
                return;
            }
        }
        {
            auto found = m_shader_map.find(path);
            if (found != m_shader_map.end())
            {
                found->second.Load();
                return;
            }
        }
        {
            auto found = m_texture_map.find(path);
            if (found != m_texture_map.end())
            {
                found->second.Load();
                return;
            }
        }
    }

    void ResourceManager::ReloadResource(Resource* resource)
    {
        ReloadPath(resource->m_file_path);
    }

    void ResourceManager::GetModelResources(std::vector<ModelResource*>& model_resources)
    {
        for (auto& [path, model_resource] : m_model_map)
        {
            model_resources.push_back(&model_resource);
        }
        std::sort(model_resources.begin(), model_resources.end(), CompareResource);
    }

    void ResourceManager::GetShaderResources(std::vector<ShaderResource*>& shader_resources)
    {
        for (auto& [path, shader_resource] : m_shader_map)
        {
            shader_resources.push_back(&shader_resource);
        }
        std::sort(shader_resources.begin(), shader_resources.end(), CompareResource);
    }

    void ResourceManager::GetTextureResources(std::vector<TextureResource*>& texture_resources)
    {
        for (auto& [path, texture_resource] : m_texture_map)
        {
            texture_resources.push_back(&texture_resource);
        }
        std::sort(texture_resources.begin(), texture_resources.end(), CompareResource);
    }

    ShaderResource* ResourceManager::GetShaderResourcePath(const String& file_path)
    {
        auto found = m_shader_map.find(file_path);
        if (found != m_shader_map.end())
        {
            return &found->second;
        }

        return nullptr;
    }

    ShaderResource* ResourceManager::GetShaderResourceName(const String& file_name)
    {
        for (auto& [path, resource] : m_shader_map)
        {
            if (resource.FileName() == file_name)
            {
                return &resource;
            }
        }
        return nullptr;
    }

    TextureResource* ResourceManager::GetTextureResourcePath(const String& file_path)
    {
        auto found = m_texture_map.find(file_path);
        if (found != m_texture_map.end())
        {
            return &found->second;
        }

        return nullptr;
    }

    TextureResource* ResourceManager::GetTextureResourceName(const String& file_name)
    {
        for (auto& [path, resource] : m_texture_map)
        {
            if (resource.FileName() == file_name)
            {
                return &resource;
            }
        }
        return nullptr;
    }

    ModelResource* ResourceManager::GetModelResourcePath(const String& file_path)
    {
        auto found = m_model_map.find(file_path);
        if (found != m_model_map.end())
        {
            return &found->second;
        }

        return nullptr;
    }

    ModelResource* ResourceManager::GetModelResourceName(const String& file_name)
    {
        for (auto& [path, resource] : m_model_map)
        {
            if (resource.FileName() == file_name)
            {
                return &resource;
            }
        }
        return nullptr;
    }

    SPtr<Texture> ResourceManager::CreateTexture(const String& name)
    {
        auto found = m_created_texture_map.find(name);
        if (found != m_created_texture_map.end())
        {
            //already created
            return nullptr;
        }

        auto texture = std::make_shared<Texture>();
        m_created_texture_map.emplace(name, texture);
        return texture;
    }

    SPtr<Texture> ResourceManager::GetCreatedTexture(const String& name) const
    {
        auto found = m_created_texture_map.find(name);
        if (found != m_created_texture_map.end())
        {
            //already created
            return found->second;
        }

        return nullptr;
    }

    void ResourceManager::Initialize()
    {
        BuildResource();
    }

    void ResourceManager::Update()
    {
    }

    void ResourceManager::Shutdown()
    {
    }

    void ResourceManager::SetRootPath(const String& path)
    {
        m_root_path = path;
    }

    void ResourceManager::QueryFilePathRecursive(const String& path, std::vector<String>& file_list, std::vector<String>& directory_list) const
    {
        std::vector<String> paths;
        FileUtility::GetFileListsInPath(path, paths);
        for (auto& file_path : paths)
        {
            if (FileUtility::IsDirectory(file_path) && FileUtility::HasNotExtension(file_path))
            {
                directory_list.push_back(file_path);
                QueryFilePathRecursive(file_path, file_list, directory_list);
            }
            else
            {
                file_list.push_back(file_path);
            }
        }
    }

    void ResourceManager::BuildResource()
    {
        m_file_list.clear();
        m_directory_list.clear();
        QueryFilePathRecursive(m_root_path, m_file_list, m_directory_list);

        for (auto& file : m_file_list)
        {
            AddResource(file);
        }
    }

    void ResourceManager::AddResource(const String& path)
    {
        Resource* resource = nullptr;
        String    type     = FileUtility::GetExtension(path);
        if (type == ".fx" || type == ".hlsl" || type == ".hlsli")
        {
            auto [pair, result] = m_shader_map.emplace(path, ShaderResource(path));
            if (result)
            {
                resource = &pair->second;
            }
        }
        else if (type == ".shader")
        {
            auto [pair, result] = m_shader_map.emplace(path, ShaderResource(path));
            if (result)
            {
                resource = &pair->second;
            }
        }
        else if (
            type == ".dds" || type == ".DDS" ||
            type == ".png" || type == ".PNG" ||
            type == ".jpeg" || type == ".JPEG" ||
            type == ".jpg" || type == ".JPG" ||
            type == ".bmp" || type == ".BMP" ||
            type == ".tiff" || type == ".TIFF" ||
            type == ".gif" || type == ".GIF")
        {
            auto [pair, result] = m_texture_map.emplace(path, TextureResource(path));
            if (result)
            {
                resource = &pair->second;
            }
        }
        else if (type == ".obj")
        {
            auto [pair, result] = m_model_map.emplace(path, ModelResource(path));
            if (result)
            {
                resource = &pair->second;
            }
        }
        else if (type == ".json")
        {
        }
        else
        {
        }

        if (resource != nullptr)
        {
            resource->Initialize();
        }
    }
}
