#pragma once
#include "../../EngineDefine.hpp"
#include "../Element/Material.hpp"

namespace Engine
{
    class MaterialManager
    {
    public:
        MaterialManager();
        ~MaterialManager();

        SPtr<Material> GetMaterial(const String& name);

    private:
        HashMap<String, SPtr<Material>> m_materials;
    };
}
