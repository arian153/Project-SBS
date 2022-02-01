#pragma once
#include <list>
#include <unordered_map>

#include "DefaultAppState.hpp"
#include "../../System.hpp"

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
        void InitializeState(AppState* app_state) const;
        void UpdateState(AppState* app_state, Real dt) const;
        void FixedUpdateState(AppState* app_state, Real dt) const;
        void RenderState(AppState* app_state, Real dt) const;
        void ShutdownState(AppState* app_state) const;

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
    private:
    };
}
