/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include <list>
#include <unordered_map>

#include "DefaultAppState.hpp"
#include "../../System.hpp"
#include "../../GUI/InAppEditor.hpp"

namespace Engine
{
    class AppStateFactory;
    class AppState;

    class AppStateManager final : public System
    {
    public:
        AppStateManager();
        ~AppStateManager() override;

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;
    public:
        void SetInitialState(const String& name);
        void SetQuit();

        //state change methods
        void ChangeState(const String& next_state);
        void RestartState();
        void PauseAndChangeState(const String& next_state);
        bool ResumeState();
        bool ResumeAndRestartState();

        String    GetCurrentStateName() const;
        AppState* GetCurrentState() const;
        void      UpdatePausedStates(Real dt);

        void AddFactory(const String& name, AppStateFactory* factory);

    private:
        void ClearFactory();

        void      RemovePausedState();
        AppState* CreateState(const String& name);

        //loop state
        static void InitializeState(AppState* app_state);
        static void UpdateState(AppState* app_state, Real dt);
        static void FixedUpdateState(AppState* app_state, Real dt);
        static void RenderState(AppState* app_state, Real dt);
        static void ShutdownState(AppState* app_state);

    private:
        struct PauseInfo
        {
            String    name;
            AppState* app_state = nullptr;
        };

    private:
        friend class CoreSystem;

    private:
        Real m_fixed_time_step = 0.02f;
        Real m_elapsed_time    = 0.0f;

        String    m_curr;
        String    m_next;
        AppState* m_app_state = nullptr;

        bool m_b_set_initial_app_state = false;
        bool m_b_quit_state_machine    = false;

        bool m_b_restart          = false;
        bool m_b_pause_and_change = false;
        bool m_b_resume           = false;
        bool m_b_resume_restart   = false;

        std::list<PauseInfo> m_pause_levels;

        HashMap<String, AppStateFactory*> m_state_factory;
        DefaultAppStateFactory            m_default_factory;

        InAppEditor m_in_app_editor;
    };
}
