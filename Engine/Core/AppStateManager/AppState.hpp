#pragma once
#include "SubsystemFlag.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class BehaviorSubsystem;
    class AnimationSubsystem;
    class PhysicsSubsystem;
    class SpaceSubsystem;
    class ComponentManager;
    class ObjectManager;
    class RenderSubsystem;
    class Subsystem;

    class AppState
    {
    public:
        virtual ~AppState();

        virtual void Initialize() = 0;
        virtual void Update(float dt) = 0;
        virtual void FixedUpdate(float dt) = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

    public:
        SPtr<ObjectManager>    GetObjectManager() const;
        SPtr<ComponentManager> GetComponentManager() const;

    protected:
        void SetDefaultSubsystems();

    private:
        void InitializeEngineSys(AppState* app_state);
        void UpdateEngineSys(float dt, eSubsystemFlag flag) const;
        void RenderEngineSys() const;
        void ShutdownEngineSys() const;

    protected:
        friend class AppStateManager;

    protected:
        String m_state_name;
        bool   m_b_activate = true;

        eSubsystemFlag m_creation_flag     = eSubsystemFlag::None;
        eSubsystemFlag m_update_flag       = eSubsystemFlag::None;
        eSubsystemFlag m_fixed_update_flag = eSubsystemFlag::None;
        eSubsystemFlag m_render_flag       = eSubsystemFlag::None;

        std::vector<UPtr<Subsystem>> m_user_systems;

        //engine manager list
        SPtr<ObjectManager>    m_object_manager;
        SPtr<ComponentManager> m_component_manager;
        //engine subsystem list
        UPtr<SpaceSubsystem>     m_space_subsystem;
        UPtr<PhysicsSubsystem>   m_physics_subsystem;
        UPtr<RenderSubsystem>    m_render_subsystem;
        UPtr<BehaviorSubsystem>  m_behavior_subsystem;
        UPtr<AnimationSubsystem> m_animation_subsystem;
    };
}
