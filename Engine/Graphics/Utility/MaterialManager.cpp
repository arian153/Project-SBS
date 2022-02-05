#include "MaterialManager.hpp"

namespace Engine
{
    MaterialManager::MaterialManager()
    {
    }

    MaterialManager::~MaterialManager()
    {
    }

  
    SPtr<Material> MaterialManager::GetMaterial(const String& name)
    {
        auto found = m_materials.find(name);
        if (found != m_materials.end())
            return found->second;

        auto material = std::make_shared<Material>();
        m_materials.emplace(name, material);
        return material;
    }
}
