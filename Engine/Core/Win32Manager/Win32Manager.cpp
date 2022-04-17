/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Win32Manager.hpp"

#include <WindowsX.h>

#include "InputManager.hpp"
#include "KeyCode.hpp"
#include "../CoreSystem.hpp"
#include "../CoreDefine.hpp"
#include "../../External/imgui/imgui.h"
#include "../../Utility/TimeUtility.hpp"
#include "../../Utility/Utility.hpp"

// Forward declare message handler from imgui_impl_win32.cpp
// ReSharper disable once CppInconsistentNaming
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace
{
    Engine::Win32Manager* g_win32_manager = nullptr;

    LRESULT CALLBACK ProcessWindow(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
            return true;

        return g_win32_manager->ProcessMessage(hwnd, msg, wparam, lparam);
    }
}

namespace Engine
{
    Win32Manager::Win32Manager()
    {
        m_application_setting.b_confine_cursor = false;
        m_application_setting.b_show_cursor    = true;
        m_application_setting.b_vsync          = false;
        m_application_setting.b_windowed       = true;

        m_application_setting.screen_width  = 1280;
        m_application_setting.screen_height = 720;
        m_application_setting.screen_scale  = 1.0f;

        m_application_setting.caption       = "GAM450-Project";
        m_window_caption                    = L"GAM450-Project";
        m_application_setting.initial_level = "";

        m_instance_handle = GetModuleHandle(nullptr);
        m_style           = Win32::WINDOWED_STYLE_COMMON;
        g_win32_manager   = this;
    }

    Win32Manager::Win32Manager(HINSTANCE instance, const ApplicationSetting& app_setting)
        : m_application_setting(app_setting), m_instance_handle(instance)
    {
        m_style         = Win32::WINDOWED_STYLE_COMMON;
        g_win32_manager = this;

        m_window_caption = ToWString(app_setting.caption);
    }

    Win32Manager::~Win32Manager()
    {
    }

    void Win32Manager::Initialize()
    {
        SetMonitorResolution();
        WNDCLASS window_class;
        //DEVMODE dm_screen_settings;
        window_class.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        window_class.lpfnWndProc   = ProcessWindow;
        window_class.cbClsExtra    = 0;
        window_class.cbWndExtra    = 0;
        window_class.hInstance     = m_instance_handle;
        window_class.hIcon         = LoadIcon(nullptr, IDI_WINLOGO);
        window_class.hCursor       = LoadCursor(nullptr, IDC_ARROW);
        window_class.hbrBackground = nullptr;//(HBRUSH)GetStockObject(BLACK_BRUSH);
        window_class.lpszMenuName  = nullptr;
        window_class.lpszClassName = L"EngineHWND";
        if (!RegisterClass(&window_class))
        {
            MessageBox(nullptr, L"Register Class Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        m_style = GetWindowedStyle();
        // Compute window rectangle dimensions based on requested client area dimensions.
        RECT window_rect = { 0, 0, m_curr_client_width, m_curr_client_height };
        int  width_start, height_start;
        AdjustAndCenterWindow(m_style, window_rect, width_start, height_start);
        int width       = window_rect.right - window_rect.left;
        int height      = window_rect.bottom - window_rect.top;
        m_window_handle = CreateWindow(
                                       L"EngineHWND", m_window_caption.c_str(), m_style,
                                       width_start, height_start, width, height, nullptr, nullptr, m_instance_handle, this
                                      );
        if (!m_window_handle)
        {
            MessageBox(nullptr, L"Create Window Failed.", nullptr, 0);
            m_b_init = false;
            return;
        }
        SetWindowMode(m_application_setting.b_windowed);
        ShowWindow(m_window_handle, SW_SHOW);
        UpdateWindow(m_window_handle);

        m_input_manager = GetCore()->GetInputManager();
        m_b_init        = true;
    }

    void Win32Manager::Update()
    {
        DispatchMessagePump();

        if (m_b_show_fps)
        {
            Uint32     fps     = static_cast<Uint32>(TimeUtility::FramePerSecond());
            StringWide caption = m_window_caption + L"    FPS: " + std::to_wstring(fps);
            SetWindowText(m_window_handle, caption.c_str());
        }
    }

    void Win32Manager::Shutdown()
    {
    }

    WindowInfo Win32Manager::GetWindowInfo() const
    {
        WindowInfo result;

        result.hwnd       = m_window_handle;
        result.height     = m_curr_client_height;
        result.width      = m_curr_client_width;
        result.b_windowed = m_application_setting.b_windowed;

        return result;
    }

    void Win32Manager::SetConfineCursor(bool b_confine_cursor)
    {
        m_application_setting.b_confine_cursor = b_confine_cursor;
        if (b_confine_cursor == true)
        {
            RECT clip;
            GetClientRect(m_window_handle, &clip);
            ClientToScreen(m_window_handle, reinterpret_cast<LPPOINT>(&clip));
            ClientToScreen(m_window_handle, reinterpret_cast<LPPOINT>(&clip.right));
            ClipCursor(&clip);
        }
        else
        {
            ClipCursor(nullptr);
        }
    }

    void Win32Manager::SetShowCursor(bool b_show_cursor)
    {
        m_application_setting.b_show_cursor = b_show_cursor;
        ShowCursor(b_show_cursor);
    }

    void Win32Manager::SetWindowMode(bool b_window_mode)
    {
        if (m_application_setting.b_windowed == false)
        {
            if (m_curr_client_width > m_monitor_screen_width)
            {
                m_curr_client_width = m_monitor_screen_width;
            }
            if (m_curr_client_height > m_monitor_screen_height)
            {
                m_curr_client_height = m_monitor_screen_height;
            }
        }
        m_application_setting.b_windowed = b_window_mode;
        if (m_application_setting.b_windowed == false)
        {
            DEVMODE dm_screen_settings;
            dm_screen_settings.dmSize = sizeof dm_screen_settings;
            EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &dm_screen_settings);
            dm_screen_settings.dmPelsWidth  = static_cast<DWORD>(m_curr_client_width);
            dm_screen_settings.dmPelsHeight = static_cast<DWORD>(m_curr_client_height);
            dm_screen_settings.dmBitsPerPel = 32;
            dm_screen_settings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
            m_style                         = Win32::FULLSCREEN_STYLE;
            if (ChangeDisplaySettings(&dm_screen_settings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
            {
                m_application_setting.b_windowed = true;
                m_style                          = GetWindowModeStyle(m_application_setting.b_windowed);
                ChangeDisplaySettings(nullptr, 0);
                SetClientResolution(m_prev_client_width, m_prev_client_height);
            }
        }
        else
        {
            ChangeDisplaySettings(nullptr, 0);
            m_style = GetWindowModeStyle(m_application_setting.b_windowed);
        }
        SetWindowLong(m_window_handle, GWL_STYLE, m_style);
        RECT rect    = { 0, 0, m_curr_client_width, m_curr_client_height };
        int  x_start = 0, y_start = 0;
        AdjustAndCenterWindow(m_style, rect, x_start, y_start);
        MoveWindow(m_window_handle, x_start, y_start, rect.right - rect.left, rect.bottom - rect.top, TRUE);
        ShowWindow(m_window_handle, SW_SHOWNORMAL);
        SetForegroundWindow(m_window_handle);
        g_core->OnFullscreen();
    }

    void Win32Manager::SetQuit(bool b_quit)
    {
        m_b_quit = b_quit;
        if (m_b_quit)
            APP_STATE_MANAGER->SetQuit();
    }

    void Win32Manager::SetClientResolution(int width, int height)
    {
        if (m_application_setting.b_windowed == false)
        {
            if (width > m_monitor_screen_width)
                return;
            if (height > m_monitor_screen_height)
                return;
        }
        m_prev_client_width  = m_curr_client_width;
        m_prev_client_height = m_curr_client_height;
        m_curr_client_width  = width;
        m_curr_client_height = height;
        SetWindowMode(m_application_setting.b_windowed);
    }

    void Win32Manager::SetMonitorResolution()
    {
        m_monitor_screen_width  = GetSystemMetrics(SM_CXSCREEN);
        m_monitor_screen_height = GetSystemMetrics(SM_CYSCREEN);
    }

    void Win32Manager::DispatchMessagePump() const
    {
        MSG msg = { nullptr };

        while (PeekMessage(&msg, m_window_handle, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    int Win32Manager::ClientWidth() const
    {
        return m_curr_client_width;
    }

    int Win32Manager::ClientHeight() const
    {
        return m_curr_client_height;
    }

    bool Win32Manager::IsFullscreen() const
    {
        return !m_application_setting.b_windowed;
    }

    bool Win32Manager::IsConfineCursor() const
    {
        return m_application_setting.b_confine_cursor;
    }

    bool Win32Manager::IsShowCursor() const
    {
        return m_application_setting.b_show_cursor;
    }

    bool Win32Manager::IsInit() const
    {
        return m_b_init;
    }

    bool Win32Manager::IsQuit() const
    {
        return m_b_quit;
    }

    bool Win32Manager::IsPaused() const
    {
        return m_b_app_paused;
    }

    bool Win32Manager::IsWindowMode() const
    {
        return m_application_setting.b_windowed;
    }

    Real Win32Manager::AspectRatio() const
    {
        return static_cast<Real>(m_curr_client_width) / static_cast<Real>(m_curr_client_height);
    }

    LRESULT Win32Manager::ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
    {
        switch (msg)
        {
        // WM_ACTIVATE is sent when the window is activated or deactivated.  
        // We pause the game when the window is deactivated and unpause it 
        // when it becomes active.  
        case WM_ACTIVATE:
            if (LOWORD(wparam) == WA_INACTIVE)
            {
                m_b_app_paused = true;
                TimeUtility::Stop();
            }
            else
            {
                m_b_app_paused = false;
                TimeUtility::Start();
                g_core->OnFullscreen();
            }
            break;
        // WM_SIZE is sent when the user resizes the window.  
        case WM_SIZE:
            // Save the new client area dimensions.
            if (!IsFullscreen())
            {
                m_prev_client_width  = m_curr_client_width;
                m_prev_client_height = m_curr_client_height;
                m_curr_client_width  = LOWORD(lparam);
                m_curr_client_height = HIWORD(lparam);
            }
            if (wparam == SIZE_MINIMIZED)
            {
                m_b_app_paused = true;
                m_b_minimized  = true;
                m_b_maximized  = false;
            }
            else if (wparam == SIZE_MAXIMIZED)
            {
                m_b_app_paused = false;
                m_b_minimized  = false;
                m_b_maximized  = true;
                g_core->OnResize();
            }
            else if (wparam == SIZE_RESTORED)
            {
                // Restoring from minimized state?
                if (m_b_minimized)
                {
                    m_b_app_paused = false;
                    m_b_minimized  = false;
                    g_core->OnResize();
                }
                // Restoring from maximized state?
                else if (m_b_maximized)
                {
                    m_b_app_paused = false;
                    m_b_maximized  = false;
                    g_core->OnResize();
                }
                else if (m_b_resizable)
                {
                }
                else
                {
                    g_core->OnResize();
                }
            }
            break;
        // WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
        case WM_ENTERSIZEMOVE:
            m_b_app_paused = true;
            m_b_resizable = true;
            TimeUtility::Stop();
            break;
        // WM_EXITSIZEMOVE is sent when the user releases the resize bars.
        // Here we reset everything based on the new window dimensions.
            [[fallthrough]];
        case WM_EXITSIZEMOVE:
            m_b_app_paused = false;
            m_b_resizable = false;
            TimeUtility::Start();
            g_core->OnResize();
            break;
        // The WM_MENUCHAR message is sent when a menu is active and the user presses 
        // a key that does not correspond to any mnemonic or accelerator key. 
        case WM_MENUCHAR:
            // Don't beep when we alt-enter.
            return MAKELRESULT(0, MNC_CLOSE);
        // Catch this message so to prevent the window from becoming too small.
        case WM_GETMINMAXINFO:
            reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.x = 200;
            reinterpret_cast<MINMAXINFO*>(lparam)->ptMinTrackSize.y = 200;
            break;
        case WM_LBUTTONUP:
            m_input_manager->ProcessMouseEvent(false, eKeyCodeMouse::Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_LBUTTONDOWN:
            m_input_manager->ProcessMouseEvent(true, eKeyCodeMouse::Left, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_window_handle);
            break;
        case WM_RBUTTONUP:
            m_input_manager->ProcessMouseEvent(false, eKeyCodeMouse::Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_RBUTTONDOWN:
            m_input_manager->ProcessMouseEvent(true, eKeyCodeMouse::Right, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_window_handle);
            break;
        case WM_MBUTTONUP:
            m_input_manager->ProcessMouseEvent(false, eKeyCodeMouse::Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_MBUTTONDOWN:
            m_input_manager->ProcessMouseEvent(true, eKeyCodeMouse::Middle, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_window_handle);
            break;
        case WM_XBUTTONUP:
            m_input_manager->ProcessMouseEvent(false, eKeyCodeMouse::X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            ReleaseCapture();
            break;
        case WM_XBUTTONDOWN:
            m_input_manager->ProcessMouseEvent(true, eKeyCodeMouse::X1, GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            SetCapture(m_window_handle);
            break;
        case WM_MOUSEMOVE:
            m_input_manager->ProcessMouseEvent(GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam));
            break;
        case WM_MOUSEWHEEL:
            m_input_manager->ProcessMouseWheel(GET_WHEEL_DELTA_WPARAM(wparam));
            break;
        // WM_DESTROY is sent when the window is being destroyed.
        case WM_DESTROY:
            GetCore()->OnQuit();
            PostQuitMessage(0);
            break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
            break;
        case WM_SYSCOMMAND:
            if ((wparam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
        default:
            return DefWindowProc(hwnd, msg, wparam, lparam);
        }
        return 0;
    }

    void Win32Manager::AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start) const
    {
        AdjustWindowRect(&size, style, false);
        x_start = (m_monitor_screen_width - size.right + size.left) / 2;
        y_start = (m_monitor_screen_height - size.bottom + size.top) / 2;
    }

    DWORD Win32Manager::GetWindowedStyle() const
    {
        return m_curr_client_height >= m_monitor_screen_height ? Win32::WINDOWED_STYLE : Win32::WINDOWED_STYLE_COMMON;
    }

    HINSTANCE Win32Manager::AppInstance() const
    {
        return m_instance_handle;
    }

    HWND Win32Manager::AppHWnd() const
    {
        return m_window_handle;
    }

    DWORD Win32Manager::GetWindowModeStyle(bool b_window_mode) const
    {
        return b_window_mode ? GetWindowedStyle() : Win32::FULLSCREEN_STYLE;
    }
}
