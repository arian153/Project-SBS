#include "ConsoleUtility.hpp"
#include <comdef.h>
#include <string>

namespace Engine
{
    void ConsoleUtility::CreateConsole()
    {
        FILE* p_file;
        AllocConsole();
        freopen_s(&p_file, "CONOUT$", "wt", stdout);
        freopen_s(&p_file, "CONOUT$", "wt", stderr);
        SetConsoleTitle(L"CS460th Console");

        auto handle = GetConsoleWindow();
        auto h_menu = GetSystemMenu(handle, false);
        DeleteMenu(h_menu, SC_CLOSE, MF_BYCOMMAND);
    }

    void ConsoleUtility::DestroyConsole()
    {
        FreeConsole();
    }

    void ConsoleUtility::ClearConsole()
    {
        // Coordinates of first cell
        COORD                      screen_coordinate = { 0, 0 };    // home for the cursor
        DWORD                      c_chars_written;
        CONSOLE_SCREEN_BUFFER_INFO console_screen_buffer;
        // Handle to console screen buffer
        HANDLE h_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        // Get the number of character cells in the current buffer.
        if (!GetConsoleScreenBufferInfo(h_stdout, &console_screen_buffer))
            return;
        DWORD dw_con_size = console_screen_buffer.dwSize.X * console_screen_buffer.dwSize.Y;
        // Fill the entire screen with blanks.
        int result = FillConsoleOutputCharacter(h_stdout, ' ', dw_con_size, screen_coordinate, &c_chars_written);
        if (!result)
            return;
        // Get the current text attribute.
        if (!GetConsoleScreenBufferInfo(h_stdout, &console_screen_buffer))
            return;
        // Set the buffer's attributes accordingly.
        result = FillConsoleOutputAttribute(h_stdout, console_screen_buffer.wAttributes, dw_con_size, screen_coordinate, &c_chars_written);  // Receive number of characters written
        if (!result)
            return;
        // Put the cursor at its home coordinates.
        SetConsoleCursorPosition(h_stdout, screen_coordinate);
    }
}
