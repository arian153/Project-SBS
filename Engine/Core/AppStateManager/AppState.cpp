#include "AppState.hpp"

#include "SubsystemFlag.hpp"
#include "../../Graphics/RenderSubsystem.hpp"
#include "../ComponentManager/ComponentManager.hpp"
#include "../ObjectManager/ObjectManager.hpp"

namespace Engine
{
    AppState::~AppState()
    {
    }

    SPtr<ObjectManager> AppState::GetObjectManager() const
    {
        return m_object_manager;
    }

    SPtr<ComponentManager> AppState::GetComponentManager() const
    {
        return m_component_manager;
    }


    void AppState::InitializeEngineSys(AppState* app_state)
    {
        if (HasFlag(m_creation_flag, eSubsystemFlag::ComponentManager))
        {
            m_component_manager = std::make_shared<ComponentManager>();
            m_component_manager->Initialize(app_state);
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::EntityManager))
        {
            m_object_manager = std::make_shared<ObjectManager>();
            m_object_manager->Initialize(app_state);
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::BehaviorSubsystem))
        {
            m_behavior_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::AnimationSubsystem))
        {
            m_animation_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::PhysicsSubsystem))
        {
            m_physics_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::RenderSubsystem))
        {
            m_render_subsystem->Initialize();
        }
    }

    void AppState::UpdateEngineSys(float dt, eSubsystemFlag flag) const
    {
        if (HasFlag(flag, eSubsystemFlag::BehaviorSubsystem))
        {
            m_behavior_subsystem->Update(dt);
        }

        if (HasFlag(flag, eSubsystemFlag::AnimationSubsystem))
        {
            m_animation_subsystem->Update(dt);
        }

        if (HasFlag(flag, eSubsystemFlag::PhysicsSubsystem))
        {
            m_physics_subsystem->Update(dt);
        }

        if (HasFlag(flag, eSubsystemFlag::RenderSubsystem))
        {
            m_render_subsystem->Update(dt);
        }
    }

    void AppState::RenderEngineSys() const
    {
        if (HasFlag(m_render_flag, eSubsystemFlag::BehaviorSubsystem))
        {
            m_behavior_subsystem->Render();
        }

        if (HasFlag(m_render_flag, eSubsystemFlag::AnimationSubsystem))
        {
            m_animation_subsystem->Render();
        }

        if (HasFlag(m_render_flag, eSubsystemFlag::PhysicsSubsystem))
        {
            m_physics_subsystem->Render();
        }

        if (HasFlag(m_render_flag, eSubsystemFlag::RenderSubsystem))
        {
            m_render_subsystem->Render();
        }
    }

    void AppState::ShutdownEngineSys() const
    {
        if (m_behavior_subsystem)
        {
            m_behavior_subsystem->Shutdown();
        }

        if (m_animation_subsystem)
        {
            m_animation_subsystem->Shutdown();
        }

        if (m_physics_subsystem)
        {
            m_physics_subsystem->Shutdown();
        }

        if (m_render_subsystem)
        {
            m_render_subsystem->Shutdown();
        }

        if (m_object_manager)
        {
            m_object_manager->Shutdown();
        }

        if (m_component_manager)
        {
            m_component_manager->Shutdown();
        }
    }
}
