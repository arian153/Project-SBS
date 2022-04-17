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
#include <windows.h>
#include "KeyCode.hpp"
#include "../../System.hpp"

namespace Engine
{
    struct MousePosition
    {
        int x = 0;
        int y = 0;
    };

    class InputManager final : public System
    {
    public:
        InputManager();
        ~InputManager() override;

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

    public:
        bool IsDown(eKeyCodeKeyboard key_code) const;
        bool IsDown(eKeyCodeMouse key_code) const;
        bool IsPressed(eKeyCodeKeyboard key_code) const;
        bool IsPressed(eKeyCodeMouse key_code) const;
        bool IsReleased(eKeyCodeKeyboard key_code) const;
        bool IsReleased(eKeyCodeMouse key_code) const;

        bool IsAnyKeyDown() const;
        bool IsAnyKeyPressed() const;
        bool IsAnyKeyReleased() const;

        eButtonState GetButtonState(eKeyCodeKeyboard key_code) const;
        eButtonState GetButtonState(eKeyCodeMouse key_code) const;
        eButtonState GetPrevButtonState(eKeyCodeKeyboard key_code) const;
        eButtonState GetPrevButtonState(eKeyCodeMouse key_code) const;

        MousePosition CurrentPosition() const;
        MousePosition PreviousPosition() const;

        int  CurrentMouseWheel() const;
        int  PreviousMouseWheel() const;
        bool IsWheelRolling() const;
        Real MouseWheelRollingDirection() const;

    private:
        friend class Win32Manager;
        friend class AppStateManager;
        friend class CoreSystem;

    private:
        void Reset();
        void ProcessMouseEvent(bool b_down, eKeyCodeMouse code, int x, int y);
        void ProcessMouseEvent(int x, int y);
        void ProcessMouseWheel(short wheel);
        void ProcessState();
        void SetWindowHandle(HWND handle);

        static eKeyCodeKeyboard TranslateKeyboardMessage(WPARAM wparam);
        static eKeyCodeMouse    TranslateMouseMessage(WPARAM wparam);

        static int TranslateMessage(eKeyCodeKeyboard key_code);
        static int TranslateMessage(eKeyCodeMouse button_code);

    private:
        constexpr static size_t max_keyboard_count = static_cast<size_t>(eKeyCodeKeyboard::Last);
        constexpr static size_t max_mouse_count    = static_cast<size_t>(eKeyCodeMouse::Last);

    private:
        MousePosition m_current_position;
        MousePosition m_previous_position;
        ButtonState   m_keyboard[max_keyboard_count];
        ButtonState   m_mouse[max_mouse_count];
        short         m_current_wheel    = 0;
        short         m_previous_wheel   = 0;
        bool          m_b_was_wheel      = false;
        bool          m_b_was_mouse_move = false;

        bool m_b_any_key_down     = false;
        bool m_b_any_key_pressed  = false;
        bool m_b_any_key_released = false;

        HWND m_hwnd = nullptr;
    private:
    };
}
