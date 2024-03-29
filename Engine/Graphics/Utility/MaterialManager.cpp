/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "MaterialManager.hpp"

#include "../GraphicsDefine.hpp"
#include "../../Core/CoreDefine.hpp"
#include "../../Core/ResourceManager/ResourceManager.hpp"

namespace Engine
{
    MaterialManager::MaterialManager()
    {
    }

    MaterialManager::~MaterialManager()
    {
    }

    void MaterialManager::Initialize()
    {
        // DirLight
        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Deferred-DirLight.shader"));
            material->SetTexture(0, RESOURCE_MANAGER->GetCreatedTexture("PositionTarget"));
            material->SetTexture(1, RESOURCE_MANAGER->GetCreatedTexture("NormalTarget"));
            m_materials.emplace("DirLight", material);
        }

        // PointLight
        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Deferred-PointLight.shader"));
            material->SetTexture(0, RESOURCE_MANAGER->GetCreatedTexture("PositionTarget"));
            material->SetTexture(1, RESOURCE_MANAGER->GetCreatedTexture("NormalTarget"));
            Vector2 res = VIEWPORT_MANAGER.GetScreenResolution();
            material->SetVector2(0, res);
            m_materials.emplace("PointLight", material);
        }

        // Final
        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Deferred-Final.shader"));
            //Slot 0 : Diffuse Material
            material->SetTexture(0, RESOURCE_MANAGER->GetCreatedTexture("DiffuseTarget"));
            //Slot 1 : Specular Material

            //Slot 2 : Diffuse Light
            material->SetTexture(2, RESOURCE_MANAGER->GetCreatedTexture("DiffuseLightTarget"));
            //Slot 3 : Specular Light
            material->SetTexture(3, RESOURCE_MANAGER->GetCreatedTexture("SpecularLightTarget"));
            m_materials.emplace("Final", material);
        }

        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Forward-Line.shader"));
            material->SetSint(0, 1);
            m_materials.emplace("PrimitiveRenderer-Line", material);
        }

        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Forward-Face.shader"));
            material->SetSint(0, 1);
            m_materials.emplace("PrimitiveRenderer-Face", material);
        }

        {
            auto material = std::make_shared<Material>();
            material->SetShader(GET_SHADER("Deferred-Primitive.shader"));
            material->SetSint(0, 1);
            m_materials.emplace("PrimitiveRenderer-Deferred", material);
        }
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
