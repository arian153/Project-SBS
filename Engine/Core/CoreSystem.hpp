#pragma once
#include "../System.hpp"

namespace Engine
{
    class PhysicsSystem;
    class BehaviorSystem;
    class AnimationSystem;
    class GUISystem;
    class ResourceManager;
    class AppStateManager;
    class InputManager;
    class Win32Manager;
    class RenderSystem;

    class CoreSystem final : public System
    {
    public:
        CoreSystem();
        ~CoreSystem() override;

        void SetInstance(void* h_instance);
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        void OnFullscreen() const;
        void OnResize() const;
        void OnQuit() const;

        Win32Manager*         GetWin32Manager() const;
        InputManager*         GetInputManager() const;
        RenderSystem*         GetRenderSystem() const;
        AppStateManager*      GetAppStateManager() const;
        ResourceManager*      GetResourceManager() const;
        GUISystem*            GetGUISystem() const;
        RPtr<PhysicsSystem>   GetPhysicsSystem() const;
        RPtr<BehaviorSystem>  GetBehaviorSystem() const;
        RPtr<AnimationSystem> GetAnimationSystem() const;

    private:
        void* m_h_instance = nullptr;

        UPtr<Win32Manager>    m_win32_manager     = nullptr;
        UPtr<InputManager>    m_input_manager     = nullptr;
        UPtr<RenderSystem>    m_render_system     = nullptr;
        UPtr<PhysicsSystem>   m_physics_system    = nullptr;
        UPtr<BehaviorSystem>  m_behavior_system   = nullptr;
        UPtr<AnimationSystem> m_animation_system  = nullptr;
        UPtr<AppStateManager> m_app_state_manager = nullptr;
        UPtr<ResourceManager> m_resource_manager  = nullptr;
        UPtr<GUISystem>       m_gui_system        = nullptr;
    };
}
