#include "AppStateManager.hpp"

#include "AppState.hpp"
#include "AppStateFactory.hpp"
#include "../CoreSystem.hpp"
#include "../../Graphics/RenderSystem.hpp"
#include "../../GUI/GUISystem.hpp"
#include "../../Utility/TimeUtility.hpp"
#include "../Win32Manager/InputManager.hpp"
#include "../Win32Manager/Win32Manager.hpp"

namespace Engine
{
    AppStateManager::AppStateManager()
    {
    }

    AppStateManager::~AppStateManager()
    {
    }

    void AppStateManager::Initialize()
    {
    }

    void AppStateManager::Update()
    {
        //init phase
        {
            if (m_b_resume == true)
            {
                m_b_resume = false;
                delete m_app_state;
                m_app_state = m_pause_levels.front().app_state;
                m_pause_levels.pop_front();
                m_curr      = m_next;
                m_b_restart = false;
                //only resume and restart case
                if (m_b_resume_restart == true)
                {
                    m_b_resume_restart = false;
                    ShutdownState(m_app_state);
                    InitializeState(m_app_state);
                }
                m_in_app_editor.SetAppState(m_app_state);
            }
            else
            {
                m_curr      = m_next;
                m_b_restart = false;
                m_app_state = CreateState(m_curr);
                InitializeState(m_app_state);
                m_in_app_editor.SetAppState(m_app_state);
            }
        }

        //update phase
        while (m_b_quit_state_machine == false && m_b_restart == false && m_curr == m_next)
        {
            TimeUtility::Tick();
            GetCore()->GetWin32Manager()->Update();
            GetCore()->GetInputManager()->Update();

            Real time_step = TimeUtility::DeltaTime();

            g_core->GetGUISystem()->BeginImGUI();

            UpdateState(m_app_state, time_step);
            m_elapsed_time += time_step;
            if (m_elapsed_time >= m_fixed_time_step)
            {
                FixedUpdateState(m_app_state, m_fixed_time_step);
                m_elapsed_time = 0.0f;
            }

            g_core->GetGUISystem()->Update();
            m_in_app_editor.Update(time_step);

            g_core->GetGUISystem()->EndImGUI();
            RenderState(m_app_state, time_step);
        }
        //shutdown phase
        {
            if (m_b_pause_and_change == true)
            {
                m_b_pause_and_change = false;
                m_app_state          = nullptr;
            }
            else
            {
                ShutdownState(m_app_state);
                delete m_app_state;
                m_app_state = nullptr;
            }

            m_in_app_editor.Reset();
            GetCore()->GetWin32Manager()->SetQuit(m_b_quit_state_machine);
        }
    }

    void AppStateManager::Shutdown()
    {
        delete m_app_state;
        m_app_state = nullptr;
        RemovePausedState();

        ClearFactory();
    }

    void AppStateManager::SetInitialState(const String& name)
    {
        if (name == "")
        {
            return;
        }

        m_b_set_initial_app_state = true;
        m_curr                    = name;
        m_next                    = name;
    }

    void AppStateManager::SetQuit()
    {
        m_b_quit_state_machine = true;
    }

    void AppStateManager::ChangeState(const String& next_state)
    {
        m_next = next_state;
        RemovePausedState();
    }

    void AppStateManager::RestartState()
    {
        m_b_restart = true;
    }

    void AppStateManager::PauseAndChangeState(const String& next_state)
    {
        if (next_state != m_curr)
        {
            m_b_pause_and_change = true;
            PauseInfo info;
            info.name      = m_curr;
            info.app_state = m_app_state;
            m_pause_levels.push_front(info);
            m_next = next_state;
        }
    }

    bool AppStateManager::ResumeState()
    {
        if (m_pause_levels.empty() == false)
        {
            m_b_resume = true;
            m_next     = m_pause_levels.front().name;
            return true;
        }
        return false;
    }

    bool AppStateManager::ResumeAndRestartState()
    {
        if (m_pause_levels.empty() == false)
        {
            m_next             = m_pause_levels.front().name;
            m_b_restart        = true;
            m_b_resume         = true;
            m_b_resume_restart = true;
            return true;
        }
        return false;
    }

    String AppStateManager::GetCurrentStateName() const
    {
        return m_curr;
    }

    AppState* AppStateManager::GetCurrentState() const
    {
        return m_app_state;
    }

    void AppStateManager::UpdatePausedStates(Real dt)
    {
        if (m_pause_levels.empty() == false)
        {
            for (auto [name, app_state] : m_pause_levels)
            {
                UpdateState(app_state, dt);
            }
        }
    }

    void AppStateManager::AddFactory(const String& name, AppStateFactory* factory)
    {
        m_state_factory.emplace(name, factory);
    }

    void AppStateManager::ClearFactory()
    {
        for (auto& [fst, snd] : m_state_factory)
        {
            delete snd;
            snd = nullptr;
        }
    }

    void AppStateManager::RemovePausedState()
    {
        while (m_pause_levels.empty() == false)
        {
            auto info = m_pause_levels.front();
            ShutdownState(info.app_state);
            delete info.app_state;
            m_pause_levels.pop_front();
        }
    }

    AppState* AppStateManager::CreateState(const String& name)
    {
        AppState* created = nullptr;
        {
            auto found = m_state_factory.find(name);
            if (found != m_state_factory.end())
            {
                created = found->second->Create();
            }
            else
            {
                //use default factory
                created = m_default_factory.Create();
            }
        }

        //find resource
        {
            /*auto   found = m_resources.find(name);
            if (found != m_resources.end())
            {
                auto resource = found->second;
                if (resource->IsLevel())
                {
                    resource->LoadData(created);
                }
            }*/
        }

        return created;
    }

    void AppStateManager::InitializeState(AppState* app_state)
    {
        app_state->InitializeEngineSys(app_state);
        app_state->Initialize();
    }

    void AppStateManager::UpdateState(AppState* app_state, Real dt)
    {
        app_state->Update(dt);
        app_state->UpdateEngineSys(dt, app_state->m_update_flag);
    }

    void AppStateManager::FixedUpdateState(AppState* app_state, Real dt)
    {
        app_state->FixedUpdate(dt);
        app_state->UpdateEngineSys(dt, app_state->m_fixed_update_flag);
    }

    void AppStateManager::RenderState(AppState* app_state, Real dt)
    {
        GetCore()->GetRenderSystem()->RenderBegin();
        app_state->Render();
        app_state->RenderEngineSys();
        GetCore()->GetGUISystem()->RenderDrawData();
        GetCore()->GetRenderSystem()->RenderEnd();
    }

    void AppStateManager::ShutdownState(AppState* app_state)
    {
        app_state->ShutdownEngineSys();
        app_state->Shutdown();
    }
}
