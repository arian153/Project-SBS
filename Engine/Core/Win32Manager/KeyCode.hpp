#pragma once
#include "../../EngineDefine.hpp"

namespace Engine
{
    enum class eKeyCodeKeyboard : Sint32
    {
        None,
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        _7,
        _8,
        _9,
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        ArrowUp,
        ArrowDown,
        ArrowRight,
        ArrowLeft,
        Enter,
        Escape,
        Backspace,
        Tab,
        Space,
        ControlLeft,
        ControlRight,
        ShiftLeft,
        ShiftRight,
        AltLeft,
        AltRight,
        Period,
        Comma,
        Slash,
        Semicolon,
        Quote,
        BracketLeft,
        BracketRight,
        Backslash,
        Minus,
        Equal,
        PrintScreen,
        ScrollLock,
        Pause,
        CapsLock,
        Insert,
        Home,
        PageUp,
        Delete,
        End,
        PageDown,
        Application,
        Power,
        Select,
        Last
    };

    enum class eKeyCodeMouse : Sint32
    {
        None,
        Left,
        Right,
        Middle,
        X1,
        X2,
        Last
    };

    enum class eButtonState
    {
        None,       // None interaction with button 
        Released, // First frame the user release the button 
        Down,     // Hold down the button
        Pressed   // First frame the user press the button 
    };

    struct ButtonState
    {
        bool b_down = false;

        eButtonState curr_state = eButtonState::None;
        eButtonState prev_state = eButtonState::None;
    };
}
