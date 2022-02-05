#include "CoreSystem.hpp"

#include "../Animation/AnimationSystem.hpp"
#include "../Behavior/BehaviorSystem.hpp"
#include "../Graphics/RenderSystem.hpp"
#include "../GUI/GUISystem.hpp"
#include "../Physics/PhysicsSystem.hpp"
#include "../Utility/TimeUtility.hpp"
#include "AppStateManager/AppStateManager.hpp"
#include "ComponentManager/ComponentRegistry.hpp"
#include "ResourceManager/ResourceManager.hpp"
#include "Win32Manager/InputManager.hpp"
#include "Win32Manager/Win32Manager.hpp"

namespace Engine
{
    CoreSystem::CoreSystem()
    {
    }

    CoreSystem::~CoreSystem()
    {
    }

    void CoreSystem::SetInstance(void* h_instance)
    {
        m_h_instance = h_instance;
    }

    void CoreSystem::Initialize()
    {
        ApplicationSetting setting;
        setting.caption       = "Test Project";
        setting.resource_path = "Resource";

        TimeUtility::Reset();

        m_win32_manager     = std::make_unique<Win32Manager>(static_cast<HINSTANCE>(m_h_instance), setting);
        m_input_manager     = std::make_unique<InputManager>();
        m_render_system     = std::make_unique<RenderSystem>();
        m_app_state_manager = std::make_unique<AppStateManager>();
        m_resource_manager  = std::make_unique<ResourceManager>();
        m_gui_system        = std::make_unique<GUISystem>();
        m_physics_system    = std::make_unique<PhysicsSystem>();
        m_behavior_system   = std::make_unique<BehaviorSystem>();
        m_animation_system  = std::make_unique<AnimationSystem>();

        ComponentRegistry::Initialize();

        m_win32_manager->Initialize();
        m_input_manager->Initialize();
        m_input_manager->SetWindowHandle(m_win32_manager->AppHWnd());

        m_render_system->SetWindowInfo(m_win32_manager->GetWindowInfo());
        m_render_system->Initialize();

        m_app_state_manager->Initialize();

        m_resource_manager->SetRootPath(setting.resource_path);
        m_resource_manager->Initialize();

        m_physics_system->Initialize();
        m_animation_system->Initialize();
        m_behavior_system->Initialize();

        m_render_system->PostInitialize();

        m_gui_system->SetUpImGUI();
    }

    void CoreSystem::Update()
    {
        while (m_win32_manager->IsQuit() == false && m_win32_manager->IsInit() == true)
        {
            m_app_state_manager->Update();
        }
    }

    void CoreSystem::Shutdown()
    {
        if (m_win32_manager != nullptr)
        {
            m_win32_manager->Shutdown();
        }

        if (m_input_manager != nullptr)
        {
            m_input_manager->Shutdown();
        }

        if (m_render_system != nullptr)
        {
            m_render_system->Shutdown();
        }

        if (m_app_state_manager != nullptr)
        {
            m_app_state_manager->Shutdown();
        }

        if (m_resource_manager != nullptr)
        {
            m_resource_manager->Shutdown();
        }

        m_gui_system->CleanUpImGUI();
        ComponentRegistry::Shutdown();
    }

    void CoreSystem::OnFullscreen() const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnFullscreen(m_win32_manager->IsFullscreen());
        }
    }

    void CoreSystem::OnResize() const
    {
        if (m_render_system != nullptr)
        {
            m_render_system->OnResize(m_win32_manager->ClientWidth(), m_win32_manager->ClientHeight());
        }
    }

    void CoreSystem::OnQuit() const
    {
        m_app_state_manager->SetQuit();
        m_win32_manager->SetQuit(true);
    }

    Win32Manager* CoreSystem::GetWin32Manager() const
    {
        return m_win32_manager.get();
    }

    InputManager* CoreSystem::GetInputManager() const
    {
        return m_input_manager.get();
    }

    RenderSystem* CoreSystem::GetRenderSystem() const
    {
        return m_render_system.get();
    }

    AppStateManager* CoreSystem::GetAppStateManager() const
    {
        return m_app_state_manager.get();
    }

    ResourceManager* CoreSystem::GetResourceManager() const
    {
        return m_resource_manager.get();
    }

    GUISystem* CoreSystem::GetGUISystem() const
    {
        return m_gui_system.get();
    }

    RPtr<PhysicsSystem> CoreSystem::GetPhysicsSystem() const
    {
        return m_physics_system.get();
    }

    RPtr<BehaviorSystem> CoreSystem::GetBehaviorSystem() const
    {
        return m_behavior_system.get();
    }

    RPtr<AnimationSystem> CoreSystem::GetAnimationSystem() const
    {
        return m_animation_system.get();
    }
}
