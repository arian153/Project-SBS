/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "InputManager.hpp"

#include "../CoreIncludes.h"

namespace Engine
{
    InputManager::InputManager()
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::Initialize()
    {
    }

    void InputManager::Update()
    {
        ProcessState();
    }

    void InputManager::Shutdown()
    {
    }

    bool InputManager::IsDown(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<size_t>(key_code)].curr_state > eButtonState::Released;
    }

    bool InputManager::IsDown(eKeyCodeMouse key_code) const
    {
        return m_mouse[static_cast<size_t>(key_code)].curr_state > eButtonState::Released;
    }

    bool InputManager::IsPressed(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<size_t>(key_code)].curr_state == eButtonState::Pressed;
    }

    bool InputManager::IsPressed(eKeyCodeMouse key_code) const
    {
        return m_mouse[static_cast<size_t>(key_code)].curr_state == eButtonState::Pressed;
    }

    bool InputManager::IsReleased(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<size_t>(key_code)].curr_state == eButtonState::Released;
    }

    bool InputManager::IsReleased(eKeyCodeMouse key_code) const
    {
        return m_mouse[static_cast<size_t>(key_code)].curr_state == eButtonState::Released;
    }

    bool InputManager::IsAnyKeyDown() const
    {
        return m_b_any_key_down;
    }

    bool InputManager::IsAnyKeyPressed() const
    {
        return m_b_any_key_pressed;
    }

    bool InputManager::IsAnyKeyReleased() const
    {
        return m_b_any_key_released;
    }

    eButtonState InputManager::GetButtonState(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<size_t>(key_code)].curr_state;
    }

    eButtonState InputManager::GetButtonState(eKeyCodeMouse key_code) const
    {
        return m_mouse[static_cast<size_t>(key_code)].curr_state;
    }

    eButtonState InputManager::GetPrevButtonState(eKeyCodeKeyboard key_code) const
    {
        return m_keyboard[static_cast<size_t>(key_code)].prev_state;
    }

    eButtonState InputManager::GetPrevButtonState(eKeyCodeMouse key_code) const
    {
        return m_mouse[static_cast<size_t>(key_code)].prev_state;
    }

    MousePosition InputManager::CurrentPosition() const
    {
        return m_current_position;
    }

    MousePosition InputManager::PreviousPosition() const
    {
        return m_previous_position;
    }

    int InputManager::CurrentMouseWheel() const
    {
        return m_current_wheel;
    }

    int InputManager::PreviousMouseWheel() const
    {
        return m_previous_wheel;
    }

    bool InputManager::IsWheelRolling() const
    {
        return m_current_wheel != m_previous_wheel;
    }

    Real InputManager::MouseWheelRollingDirection() const
    {
        int wheel_value = m_current_wheel - m_previous_wheel;
        return static_cast<Real>(wheel_value) / fabsf(static_cast<Real>(wheel_value));
    }

    void InputManager::Reset()
    {
        for (size_t i = 0; i < max_keyboard_count; ++i)
        {
            m_keyboard[i].b_down     = false;
            m_keyboard[i].curr_state = eButtonState::None;
            m_keyboard[i].prev_state = eButtonState::None;
        }

        for (size_t i = 0; i < max_mouse_count; ++i)
        {
            m_mouse[i].b_down     = false;
            m_mouse[i].curr_state = eButtonState::None;
            m_mouse[i].prev_state = eButtonState::None;
        }
        m_current_wheel      = 0;
        m_previous_wheel     = 0;
        m_b_was_wheel        = false;
        m_b_was_mouse_move   = false;
        m_b_any_key_pressed  = false;
        m_b_any_key_released = false;
        m_b_any_key_down     = false;
    }

    void InputManager::ProcessMouseEvent(bool b_down, eKeyCodeMouse code, int x, int y)
    {
        m_mouse[static_cast<size_t>(code)].b_down = b_down;

        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void InputManager::ProcessMouseEvent(int x, int y)
    {
        m_previous_position.x = m_current_position.x;
        m_previous_position.y = m_current_position.y;
        m_current_position.x  = x;
        m_current_position.y  = y;
        m_b_was_mouse_move    = true;
    }

    void InputManager::ProcessMouseWheel(short wheel)
    {
        m_previous_wheel = m_current_wheel;
        m_current_wheel += wheel;
        m_b_was_wheel = true;
    }

    void InputManager::ProcessState()
    {
        HWND hwnd            = ::GetActiveWindow();
        m_b_any_key_pressed  = false;
        m_b_any_key_released = false;
        m_b_any_key_down     = false;
        if (m_hwnd != hwnd)
        {
            for (size_t key = 0; key < max_keyboard_count; key++)
            {
                m_keyboard[key].prev_state = m_keyboard[key].curr_state = eButtonState::None;
            }

            for (size_t key = 0; key < max_mouse_count; key++)
            {
                m_mouse[key].prev_state = m_mouse[key].curr_state = eButtonState::None;
            }

            return;
        }

        BYTE ascii_keys[256] = {};
        if (::GetKeyboardState(ascii_keys) == false)
            return;

        for (size_t key = 0; key < max_keyboard_count; key++)
        {
            m_keyboard[key].prev_state = m_keyboard[key].curr_state;
            eButtonState& curr_state   = m_keyboard[key].curr_state;

            if (ascii_keys[TranslateMessage(static_cast<eKeyCodeKeyboard>(key))] & 0x80)
            {
                curr_state = curr_state == eButtonState::Down || curr_state == eButtonState::Pressed
                                 ? eButtonState::Down
                                 : eButtonState::Pressed;
            }
            else
            {
                curr_state = curr_state == eButtonState::Down || curr_state == eButtonState::Pressed
                                 ? eButtonState::Released
                                 : eButtonState::None;
            }

            if (curr_state == eButtonState::Pressed)
            {
                m_b_any_key_pressed = true;
                m_b_any_key_down    = true;
            }
            else if (curr_state == eButtonState::Released)
            {
                m_b_any_key_released = true;
            }
            else if (curr_state == eButtonState::Down)
            {
                m_b_any_key_down = true;
            }
        }

        for (size_t key = 0; key < max_mouse_count; key++)
        {
            m_mouse[key].prev_state  = m_mouse[key].curr_state;
            eButtonState& curr_state = m_mouse[key].curr_state;

            if (m_mouse[key].b_down)
            {
                curr_state = curr_state == eButtonState::Down || curr_state == eButtonState::Pressed
                                 ? eButtonState::Down
                                 : eButtonState::Pressed;
            }
            else
            {
                curr_state = curr_state == eButtonState::Down || curr_state == eButtonState::Pressed
                                 ? eButtonState::Released
                                 : eButtonState::None;
            }

            if (curr_state == eButtonState::Pressed)
            {
                m_b_any_key_pressed = true;
                m_b_any_key_down    = true;
            }
            else if (curr_state == eButtonState::Released)
            {
                m_b_any_key_released = true;
            }
            else if (curr_state == eButtonState::Down)
            {
                m_b_any_key_down = true;
            }
        }

        if (m_b_was_wheel == true)
        {
            m_b_was_wheel = false;
        }
        else
        {
            if (m_current_wheel != m_previous_wheel)
            {
                m_previous_wheel = m_current_wheel;
            }
        }

        if (m_b_was_mouse_move == true)
        {
            m_b_was_mouse_move = false;
            return;
        }

        if ((m_current_position.x != m_previous_position.x || m_current_position.y != m_previous_position.y))
        {
            m_previous_position.x = m_current_position.x;
            m_previous_position.y = m_current_position.y;
        }
    }

    void InputManager::SetWindowHandle(HWND handle)
    {
        m_hwnd = handle;
    }

    eKeyCodeKeyboard InputManager::TranslateKeyboardMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_RETURN:
            return eKeyCodeKeyboard::Enter;
        case VK_ESCAPE:
            return eKeyCodeKeyboard::Escape;
        case VK_BACK:
            return eKeyCodeKeyboard::Backspace;
        case VK_TAB:
            return eKeyCodeKeyboard::Tab;
        case VK_SPACE:
            return eKeyCodeKeyboard::Space;
        case VK_OEM_COMMA:
            return eKeyCodeKeyboard::Comma;
        case VK_OEM_MINUS:
            return eKeyCodeKeyboard::Minus;
        case VK_OEM_PERIOD:
            return eKeyCodeKeyboard::Period;
        case 0x30:
            return eKeyCodeKeyboard::_0;
        case 0x31:
            return eKeyCodeKeyboard::_1;
        case 0x32:
            return eKeyCodeKeyboard::_2;
        case 0x33:
            return eKeyCodeKeyboard::_3;
        case 0x34:
            return eKeyCodeKeyboard::_4;
        case 0x35:
            return eKeyCodeKeyboard::_5;
        case 0x36:
            return eKeyCodeKeyboard::_6;
        case 0x37:
            return eKeyCodeKeyboard::_7;
        case 0x38:
            return eKeyCodeKeyboard::_8;
        case 0x39:
            return eKeyCodeKeyboard::_9;
        case VK_CAPITAL:
            return eKeyCodeKeyboard::CapsLock;
        case 0x41:
            return eKeyCodeKeyboard::A;
        case 0x42:
            return eKeyCodeKeyboard::B;
        case 0x43:
            return eKeyCodeKeyboard::C;
        case 0x44:
            return eKeyCodeKeyboard::D;
        case 0x45:
            return eKeyCodeKeyboard::E;
        case 0x46:
            return eKeyCodeKeyboard::F;
        case 0x47:
            return eKeyCodeKeyboard::G;
        case 0x48:
            return eKeyCodeKeyboard::H;
        case 0x49:
            return eKeyCodeKeyboard::I;
        case 0x4A:
            return eKeyCodeKeyboard::J;
        case 0x4B:
            return eKeyCodeKeyboard::K;
        case 0x4C:
            return eKeyCodeKeyboard::L;
        case 0x4D:
            return eKeyCodeKeyboard::M;
        case 0x4E:
            return eKeyCodeKeyboard::N;
        case 0x4F:
            return eKeyCodeKeyboard::O;
        case 0x50:
            return eKeyCodeKeyboard::P;
        case 0x51:
            return eKeyCodeKeyboard::Q;
        case 0x52:
            return eKeyCodeKeyboard::R;
        case 0x53:
            return eKeyCodeKeyboard::S;
        case 0x54:
            return eKeyCodeKeyboard::T;
        case 0x55:
            return eKeyCodeKeyboard::U;
        case 0x56:
            return eKeyCodeKeyboard::V;
        case 0x57:
            return eKeyCodeKeyboard::W;
        case 0x58:
            return eKeyCodeKeyboard::X;
        case 0x59:
            return eKeyCodeKeyboard::Y;
        case 0x5A:
            return eKeyCodeKeyboard::Z;
        case VK_F1:
            return eKeyCodeKeyboard::F1;
        case VK_F2:
            return eKeyCodeKeyboard::F2;
        case VK_F3:
            return eKeyCodeKeyboard::F3;
        case VK_F4:
            return eKeyCodeKeyboard::F4;
        case VK_F5:
            return eKeyCodeKeyboard::F5;
        case VK_F6:
            return eKeyCodeKeyboard::F6;
        case VK_F7:
            return eKeyCodeKeyboard::F7;
        case VK_F8:
            return eKeyCodeKeyboard::F8;
        case VK_F9:
            return eKeyCodeKeyboard::F9;
        case VK_F10:
            return eKeyCodeKeyboard::F10;
        case VK_F11:
            return eKeyCodeKeyboard::F11;
        case VK_F12:
            return eKeyCodeKeyboard::F12;
        case VK_SNAPSHOT:
            return eKeyCodeKeyboard::PrintScreen;
        case VK_SCROLL:
            return eKeyCodeKeyboard::ScrollLock;
        case VK_PAUSE:
            return eKeyCodeKeyboard::Pause;
        case VK_INSERT:
            return eKeyCodeKeyboard::Insert;
        case VK_HOME:
            return eKeyCodeKeyboard::Home;
        case VK_PRIOR:
            return eKeyCodeKeyboard::PageUp;
        case VK_DELETE:
            return eKeyCodeKeyboard::Delete;
        case VK_END:
            return eKeyCodeKeyboard::End;
        case VK_NEXT:
            return eKeyCodeKeyboard::PageDown;
        case VK_RIGHT:
            return eKeyCodeKeyboard::ArrowRight;
        case VK_LEFT:
            return eKeyCodeKeyboard::ArrowLeft;
        case VK_DOWN:
            return eKeyCodeKeyboard::ArrowDown;
        case VK_UP:
            return eKeyCodeKeyboard::ArrowUp;
        case VK_APPS:
            return eKeyCodeKeyboard::Application;
        case VK_LMENU:
            return eKeyCodeKeyboard::AltLeft;
        case VK_RMENU:
            return eKeyCodeKeyboard::AltRight;
        case VK_SELECT:
            return eKeyCodeKeyboard::Select;
        case VK_LCONTROL:
            return eKeyCodeKeyboard::ControlLeft;
        case VK_LSHIFT:
            return eKeyCodeKeyboard::ShiftLeft;
        case VK_RCONTROL:
            return eKeyCodeKeyboard::ControlRight;
        case VK_RSHIFT:
            return eKeyCodeKeyboard::ShiftRight;
        default:
            return eKeyCodeKeyboard::None;
        }
    }

    eKeyCodeMouse InputManager::TranslateMouseMessage(WPARAM wparam)
    {
        switch (wparam)
        {
        case VK_LBUTTON:
            return eKeyCodeMouse::Left;
        case VK_RBUTTON:
            return eKeyCodeMouse::Right;
        case VK_MBUTTON:
            return eKeyCodeMouse::Middle;
        case VK_XBUTTON1:
            return eKeyCodeMouse::X1;
        case VK_XBUTTON2:
            return eKeyCodeMouse::X2;
        default:
            return eKeyCodeMouse::None;
        }
    }

    int InputManager::TranslateMessage(eKeyCodeKeyboard key_code)
    {
        switch (key_code)
        {
        case eKeyCodeKeyboard::None:
            return 0;
        case eKeyCodeKeyboard::_0:
            return 0x30;
        case eKeyCodeKeyboard::_1:
            return 0x31;
        case eKeyCodeKeyboard::_2:
            return 0x32;
        case eKeyCodeKeyboard::_3:
            return 0x33;
        case eKeyCodeKeyboard::_4:
            return 0x34;
        case eKeyCodeKeyboard::_5:
            return 0x35;
        case eKeyCodeKeyboard::_6:
            return 0x36;
        case eKeyCodeKeyboard::_7:
            return 0x37;
        case eKeyCodeKeyboard::_8:
            return 0x38;
        case eKeyCodeKeyboard::_9:
            return 0x39;
        case eKeyCodeKeyboard::A:
            return 0x41;
        case eKeyCodeKeyboard::B:
            return 0x42;
        case eKeyCodeKeyboard::C:
            return 0x43;
        case eKeyCodeKeyboard::D:
            return 0x44;
        case eKeyCodeKeyboard::E:
            return 0x45;
        case eKeyCodeKeyboard::F:
            return 0x46;
        case eKeyCodeKeyboard::G:
            return 0x47;
        case eKeyCodeKeyboard::H:
            return 0x48;
        case eKeyCodeKeyboard::I:
            return 0x49;
        case eKeyCodeKeyboard::J:
            return 0x4A;
        case eKeyCodeKeyboard::K:
            return 0x4B;
        case eKeyCodeKeyboard::L:
            return 0x4C;
        case eKeyCodeKeyboard::M:
            return 0x4D;
        case eKeyCodeKeyboard::N:
            return 0x4E;
        case eKeyCodeKeyboard::O:
            return 0x4F;
        case eKeyCodeKeyboard::P:
            return 0x50;
        case eKeyCodeKeyboard::Q:
            return 0x51;
        case eKeyCodeKeyboard::R:
            return 0x52;
        case eKeyCodeKeyboard::S:
            return 0x53;
        case eKeyCodeKeyboard::T:
            return 0x54;
        case eKeyCodeKeyboard::U:
            return 0x55;
        case eKeyCodeKeyboard::V:
            return 0x56;
        case eKeyCodeKeyboard::W:
            return 0x57;
        case eKeyCodeKeyboard::X:
            return 0x58;
        case eKeyCodeKeyboard::Y:
            return 0x59;
        case eKeyCodeKeyboard::Z:
            return 0x5A;
        case eKeyCodeKeyboard::F1:
            return VK_F1;
        case eKeyCodeKeyboard::F2:
            return VK_F2;
        case eKeyCodeKeyboard::F3:
            return VK_F3;
        case eKeyCodeKeyboard::F4:
            return VK_F4;
        case eKeyCodeKeyboard::F5:
            return VK_F5;
        case eKeyCodeKeyboard::F6:
            return VK_F6;
        case eKeyCodeKeyboard::F7:
            return VK_F7;
        case eKeyCodeKeyboard::F8:
            return VK_F8;
        case eKeyCodeKeyboard::F9:
            return VK_F9;
        case eKeyCodeKeyboard::F10:
            return VK_F10;
        case eKeyCodeKeyboard::F11:
            return VK_F11;
        case eKeyCodeKeyboard::F12:
            return VK_F12;
        case eKeyCodeKeyboard::ArrowUp:
            return VK_UP;
        case eKeyCodeKeyboard::ArrowDown:
            return VK_DOWN;
        case eKeyCodeKeyboard::ArrowRight:
            return VK_RIGHT;
        case eKeyCodeKeyboard::ArrowLeft:
            return VK_LEFT;
        case eKeyCodeKeyboard::Enter:
            return VK_RETURN;
        case eKeyCodeKeyboard::Escape:
            return VK_ESCAPE;
        case eKeyCodeKeyboard::Backspace:
            return VK_BACK;
        case eKeyCodeKeyboard::Tab:
            return VK_TAB;
        case eKeyCodeKeyboard::Space:
            return VK_SPACE;
        case eKeyCodeKeyboard::ControlLeft:
            return VK_LCONTROL;
        case eKeyCodeKeyboard::ControlRight:
            return VK_RCONTROL;
        case eKeyCodeKeyboard::ShiftLeft:
            return VK_LSHIFT;
        case eKeyCodeKeyboard::ShiftRight:
            return VK_RSHIFT;
        case eKeyCodeKeyboard::AltLeft:
            return VK_LMENU;
        case eKeyCodeKeyboard::AltRight:
            return VK_RMENU;
        case eKeyCodeKeyboard::Quote:
            return 0;
        case eKeyCodeKeyboard::Comma:
            return VK_OEM_COMMA;
        case eKeyCodeKeyboard::Minus:
            return VK_OEM_MINUS;
        case eKeyCodeKeyboard::Period:
            return VK_OEM_PERIOD;
        case eKeyCodeKeyboard::Slash:
            return 0;
        case eKeyCodeKeyboard::Semicolon:
            return 0;
        case eKeyCodeKeyboard::Equal:
            return 0;
        case eKeyCodeKeyboard::BracketLeft:
            return 0;
        case eKeyCodeKeyboard::BracketRight:
            return 0;
        case eKeyCodeKeyboard::Backslash:
            return 0;
        case eKeyCodeKeyboard::PrintScreen:
            return VK_SNAPSHOT;
        case eKeyCodeKeyboard::ScrollLock:
            return VK_SCROLL;
        case eKeyCodeKeyboard::Pause:
            return VK_PAUSE;
        case eKeyCodeKeyboard::CapsLock:
            return VK_CAPITAL;
        case eKeyCodeKeyboard::Insert:
            return VK_INSERT;
        case eKeyCodeKeyboard::Home:
            return VK_HOME;
        case eKeyCodeKeyboard::PageUp:
            return VK_PRIOR;
        case eKeyCodeKeyboard::Delete:
            return VK_DELETE;
        case eKeyCodeKeyboard::End:
            return VK_END;
        case eKeyCodeKeyboard::PageDown:
            return VK_NEXT;
        case eKeyCodeKeyboard::Application:
            return VK_APPS;
        case eKeyCodeKeyboard::Power:
            return 0;
        case eKeyCodeKeyboard::Select:
            return VK_SELECT;
        case eKeyCodeKeyboard::Last:
            return 0;
        default:
            return 0;
        }
    }

    int InputManager::TranslateMessage(eKeyCodeMouse button_code)
    {
        switch (button_code)
        {
        case eKeyCodeMouse::None:
            return 0;
        case eKeyCodeMouse::Left:
            return VK_LBUTTON;
        case eKeyCodeMouse::Right:
            return VK_RBUTTON;
        case eKeyCodeMouse::Middle:
            return VK_MBUTTON;
        case eKeyCodeMouse::X1:
            return VK_XBUTTON1;
        case eKeyCodeMouse::X2:
            return VK_XBUTTON2;
        case eKeyCodeMouse::Last:
            return 0;
        default:
            return 0;
        }
    }
}
