#include "AppState.hpp"

#include "../CoreSystem.hpp"
#include "../../Animation/AnimationSystem.hpp"
#include "../../Behavior/BehaviorSystem.hpp"
#include "../../Graphics/RenderSystem.hpp"
#include "../../Physics/PhysicsSystem.hpp"

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

    RPtr<PhysicsSubsystem> AppState::GetPhysicsSubsystem() const
    {
        return m_physics_subsystem.get();
    }

    RPtr<RenderSubsystem> AppState::GetRenderSubsystem() const
    {
        return m_render_subsystem.get();
    }

    RPtr<BehaviorSubsystem> AppState::GetBehaviorSubsystem() const
    {
        return m_behavior_subsystem.get();
    }

    RPtr<AnimationSubsystem> AppState::GetAnimationSubsystem() const
    {
        return m_animation_subsystem.get();
    }

    void AppState::SetDefaultSubsystems()
    {
        m_creation_flag = eSubsystemFlag::ComponentManager | eSubsystemFlag::ObjectManager |
                eSubsystemFlag::RenderSubsystem | eSubsystemFlag::BehaviorSubsystem |
                eSubsystemFlag::PhysicsSubsystem | eSubsystemFlag::AnimationSubsystem;

        m_update_flag       = eSubsystemFlag::RenderSubsystem | eSubsystemFlag::BehaviorSubsystem;
        m_fixed_update_flag = eSubsystemFlag::PhysicsSubsystem | eSubsystemFlag::AnimationSubsystem;
        m_render_flag       = eSubsystemFlag::RenderSubsystem | eSubsystemFlag::BehaviorSubsystem |
                eSubsystemFlag::PhysicsSubsystem | eSubsystemFlag::AnimationSubsystem;
    }

    void AppState::InitializeEngineSys(AppState* app_state)
    {
        if (HasFlag(m_creation_flag, eSubsystemFlag::ComponentManager))
        {
            m_component_manager = std::make_shared<ComponentManager>();
            m_component_manager->Initialize(app_state);
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::ObjectManager))
        {
            m_object_manager = std::make_shared<ObjectManager>();
            m_object_manager->Initialize(app_state);
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::BehaviorSubsystem))
        {
            m_behavior_subsystem = g_core->GetBehaviorSystem()->CreateSubsystem();
            m_behavior_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::AnimationSubsystem))
        {
            m_animation_subsystem = g_core->GetAnimationSystem()->CreateSubsystem();
            m_animation_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::PhysicsSubsystem))
        {
            m_physics_subsystem = g_core->GetPhysicsSystem()->CreateSubsystem();
            m_physics_subsystem->Initialize();
        }

        if (HasFlag(m_creation_flag, eSubsystemFlag::RenderSubsystem))
        {
            m_render_subsystem = g_core->GetRenderSystem()->CreateSubsystem();
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
