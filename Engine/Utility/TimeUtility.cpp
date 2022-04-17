/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "TimeUtility.hpp"

namespace Engine
{
    TimeUtility::TimeUtility()
    {
    }

    TimeUtility::~TimeUtility()
    {
    }

    Real TimeUtility::TotalTime()
    {
        std::chrono::duration<Real64> total_time = m_b_stopped ? m_stop_point - m_base_point : m_curr_point - m_base_point;
        return static_cast<Real>(total_time.count() - m_paused_duration.count());
    }

    Real TimeUtility::DeltaTime()
    {
        return static_cast<Real>(m_delta_time);
    }

  
    Real TimeUtility::FramePerSecond()
    {
        return m_curr_fps;
    }

    Real TimeUtility::MillisecondPerFrame()
    {
        return m_curr_mspf;
    }

    Real TimeUtility::SecondPerFrame()
    {
        return (1.0f / m_curr_fps);
    }

    void TimeUtility::Reset()
    {
        m_curr_point      = std::chrono::high_resolution_clock::now();
        m_base_point      = m_curr_point;
        m_prev_point      = m_curr_point;
        m_stop_point      = std::chrono::time_point<std::chrono::high_resolution_clock>();
        m_b_stopped       = false;
        m_paused_duration = std::chrono::duration<Real64>::zero();
        m_frame_count     = 0;
    }

    void TimeUtility::Start()
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        if (m_b_stopped == true)
        {
            m_paused_duration += start_time - m_stop_point;
            m_prev_point = start_time;
            m_stop_point = std::chrono::time_point<std::chrono::high_resolution_clock>();
            m_b_stopped  = false;
        }
    }

    void TimeUtility::Stop()
    {
        if (m_b_stopped == false)
        {
            m_stop_point = std::chrono::high_resolution_clock::now();
            m_b_stopped  = true;
        }
    }

    void TimeUtility::Tick()
    {
        if (m_b_stopped == true)
        {
            m_delta_time = 0.0;
            return;
        }
        m_curr_point    = std::chrono::high_resolution_clock::now();
        m_tick_duration = m_curr_point - m_prev_point;
        m_delta_time    = m_tick_duration.count();
        m_delta_time    = m_delta_time > 0.0 ? m_delta_time : 0.0;
        m_prev_point    = m_curr_point;

        m_frame_count++;
        Real total_time = TotalTime();
        // Compute averages over one second period.
        if (total_time - m_elapsed_time >= 1.0f)
        {
            // fps = frame count / 1 second
            m_curr_fps  = static_cast<Real>(m_frame_count);
            m_curr_mspf = 1000.0f / m_curr_fps;
            // Reset for next average.
            m_frame_count = 0;
            m_elapsed_time += 1.0f;
        }
    }

    void TimeUtility::RecordBegin()
    {
        m_begin_point = std::chrono::high_resolution_clock::now();
    }

    Real TimeUtility::RecordEnd()
    {
        m_end_point   = std::chrono::high_resolution_clock::now();
        auto duration = m_end_point - m_begin_point;
        return std::chrono::duration<Real>(duration).count();
    }
}
