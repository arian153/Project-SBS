/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Utility.hpp"
#include <comdef.h>
#include <sstream>
#include <vector>
#include <Windows.h>

namespace Engine
{
    StringWide ToWString(const String& str)
    {
        StringWide converted_string;
        int     required_size = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
        if (required_size > 0)
        {
            std::vector<wchar_t> buffer(required_size);
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], required_size);
            converted_string.assign(buffer.begin(), buffer.end() - 1);
        }
        return converted_string;
    }

    String ToString(const StringWide& wstr)
    {
        String converted_string;
        int    required_size = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
        if (required_size > 0)
        {
            std::vector<char> buffer(required_size);
            WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &buffer[0], required_size, nullptr, nullptr);
            converted_string.assign(buffer.begin(), buffer.end() - 1);
        }
        return converted_string;
    }
}
