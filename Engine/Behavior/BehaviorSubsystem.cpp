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
