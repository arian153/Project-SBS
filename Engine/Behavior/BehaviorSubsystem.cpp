#include "BehaviorSubsystem.hpp"

#include "../Core/ComponentManager/Components/TransformComp.hpp"

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
    }

    void BehaviorSubsystem::AddTransform(RPtr<TransformCompo> compo)
    {
        size_t index = m_transforms.size();
        m_transforms.push_back(Transform());
        //compo->m_transform = &m_transforms[index];
    }

    void BehaviorSubsystem::RemoveTransform(RPtr<TransformCompo> compo)
    {
    }
}
