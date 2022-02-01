#pragma once
#include <windows.h>

#include "../ApplicationSetting.hpp"
#include "../../System.hpp"

namespace Engine::Win32
{
    constexpr DWORD FULLSCREEN_STYLE      = WS_POPUP | WS_VISIBLE;
    constexpr DWORD WINDOWED_STYLE        = WS_POPUP | WS_CAPTION;
    constexpr DWORD WINDOWED_STYLE_COMMON = WS_OVERLAPPEDWINDOW;
    constexpr DWORD BORDERLESS_STYLE      = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP;
}

namespace Engine
{
    class InputManager;

    class Win32Manager final : public System
    {
    public:
        Win32Manager();
        explicit Win32Manager(HINSTANCE instance, const ApplicationSetting& app_setting);
        ~Win32Manager() override;

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

    public:
        WindowInfo GetWindowInfo() const;

        void SetConfineCursor(bool b_confine_cursor);
        void SetShowCursor(bool b_show_cursor);
        void SetWindowMode(bool b_window_mode);
        void SetQuit(bool b_quit);
        void SetClientResolution(int width, int height);
     
        int  ClientWidth() const;
        int  ClientHeight() const;
        bool IsFullscreen() const;
        bool IsConfineCursor() const;
        bool IsShowCursor() const;
        bool IsInit() const;
        bool IsQuit() const;
        bool IsPaused() const;
        bool IsWindowMode() const;
        Real AspectRatio() const;

        LRESULT ProcessMessage(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

        HINSTANCE AppInstance() const;
        HWND      AppHWnd() const;

    protected:
        void SetMonitorResolution();
        DWORD GetWindowModeStyle(bool b_window_mode) const;
        void DispatchMessagePump() const;
        void    AdjustAndCenterWindow(DWORD style, RECT& size, int& x_start, int& y_start) const;
        DWORD   GetWindowedStyle() const;

    private:
        friend class AppStateManager;
        friend class CoreSystem;


    private:
        InputManager* m_input_manager = nullptr;

        ApplicationSetting m_application_setting;
        StringWide         m_window_caption;

        //window style option
        bool m_b_resizable = false;

        //window application state
        bool m_b_loaded     = false;
        bool m_b_init       = false;
        bool m_b_app_paused = false;
        bool m_b_minimized  = false;
        bool m_b_maximized  = false;
        bool m_b_quit       = false;
        bool m_b_show_fps   = true;

        //application window size
        int m_curr_client_width  = 1280;
        int m_curr_client_height = 720;
        int m_prev_client_width  = 1280;
        int m_prev_client_height = 720;

        //actual monitor resolution
        int m_monitor_screen_width  = 0;
        int m_monitor_screen_height = 0;

        //window data
        HINSTANCE m_instance_handle = nullptr;
        HWND      m_window_handle   = nullptr;
        DWORD     m_style           = Win32::WINDOWED_STYLE_COMMON;
    };
}
