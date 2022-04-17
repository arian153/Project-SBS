/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "BehaviorSubsystem.hpp"

#include "../Core/ComponentManager/Components/TransformCompo.hpp"

namespace Engine
{
    BehaviorSubsystem::BehaviorSubsystem()
    {
    }

    BehaviorSubsystem::~BehaviorSubsystem()
    {
    }

    void BehaviorSubsystem::Initialize()
    {
    }

    void BehaviorSubsystem::Update(Real dt)
    {
    }

    void BehaviorSubsystem::Render()
    {
    }

    void BehaviorSubsystem::Shutdown()
    {
        m_transforms.clear();
    }

    void BehaviorSubsystem::Edit()
    {

    }

    void BehaviorSubsystem::AddTransform(RPtr<TransformCompo> compo)
    {
        m_transforms.push_back(compo);
    }

    void BehaviorSubsystem::RemoveTransform(RPtr<TransformCompo> compo)
    {
        auto found = std::find(m_transforms.begin(), m_transforms.end(), compo);
        if (found != m_transforms.end())
        {
            m_transforms.erase(found);
        }
    }
}
