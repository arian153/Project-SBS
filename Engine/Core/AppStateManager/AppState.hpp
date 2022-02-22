#pragma once
#include "../../EngineDefine.hpp"

#include "SubsystemFlag.hpp"
#include "../../Animation/AnimationSubsystem.hpp"
#include "../../Behavior/BehaviorSubsystem.hpp"
#include "../../Graphics/RenderSubsystem.hpp"
#include "../../Physics/PhysicsSubsystem.hpp"
#include "../ComponentManager/ComponentManager.hpp"
#include "../ObjectManager/ObjectManager.hpp"

namespace Engine
{
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
        SPtr<ObjectManager>      GetObjectManager() const;
        SPtr<ComponentManager>   GetComponentManager() const;
        RPtr<PhysicsSubsystem>   GetPhysicsSubsystem() const;
        RPtr<RenderSubsystem>    GetRenderSubsystem() const;
        RPtr<BehaviorSubsystem>  GetBehaviorSubsystem() const;
        RPtr<AnimationSubsystem> GetAnimationSubsystem() const;

    protected:
        void SetDefaultSubsystems();

    private:
        void InitializeEngineSys(AppState* app_state);
        void UpdateEngineSys(float dt, eSubsystemFlag flag) const;
        void RenderEngineSys() const;
        void ShutdownEngineSys() const;


    protected:
        friend class AppStateManager;
        friend class InAppEditor;

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
        SPtr<PhysicsSubsystem>   m_physics_subsystem;
        SPtr<RenderSubsystem>    m_render_subsystem;
        SPtr<BehaviorSubsystem>  m_behavior_subsystem;
        SPtr<AnimationSubsystem> m_animation_subsystem;
    };
}
