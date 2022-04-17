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
#include <chrono>

#include "../EngineDefine.hpp"

namespace Engine
{
    class TimeUtility
    {
    public:
        TimeUtility();
        ~TimeUtility();

        static Real TotalTime();
        static Real DeltaTime();

        static Real FramePerSecond();
        static Real MillisecondPerFrame();
        static Real SecondPerFrame();

        void RecordBegin();
        Real RecordEnd();

    private:
        static void Reset();
        static void Start();
        static void Stop();
        static void Tick();

    private:
        friend class AppStateManager;
        friend class Win32Manager;
        friend class CoreSystem;

    private:
        //time data
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_base_point;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_prev_point;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_curr_point;
        inline static std::chrono::time_point<std::chrono::high_resolution_clock> m_stop_point;
        //duration
        inline static std::chrono::duration<Real64> m_tick_duration;
        inline static std::chrono::duration<Real64> m_paused_duration;

        //others
        inline static Real64 m_delta_time   = -1.0;
        inline static bool   m_b_stopped    = false;
        inline static Uint32 m_frame_count  = 0;
        inline static Real   m_elapsed_time = 0.0f;
        inline static Real   m_curr_fps     = 0.0f;
        inline static Real   m_curr_mspf    = 0.0f;

        std::chrono::time_point<std::chrono::high_resolution_clock> m_begin_point;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_end_point;
    };

    //extern TimeUtility g_time_utility;
}
