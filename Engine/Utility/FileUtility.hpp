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

#include "../EngineDefine.hpp"

namespace Engine
{
    class FileUtility
    {
    public:
        FileUtility()  = default;
        ~FileUtility() = default;

        static String GetPath(const StringWide& path);
        static String GetExtension(const StringWide& path);
        static String GetExtension(const String& path);
        static String GetFileName(const StringWide& path);
        static String GetFileName(const String& path);

        static void GetFileListsInPath(const StringWide& path, std::vector<StringWide>& list);
        static void GetFileListsInPath(const StringWide& path, std::vector<String>& list);
        static void GetFileListsInPath(const String& path, std::vector<String>& list);

        static void GetFileListsInPathRecursive(const StringWide& path, std::vector<StringWide>& list);
        static void GetFileListsInPathRecursive(const StringWide& path, std::vector<String>& list);
        static void GetFileListsInPathRecursive(const String& path, std::vector<String>& list);

        static bool IsDirectory(const StringWide& path);
        static bool IsDirectory(const String& path);
        static bool HasExtension(const String& path);
        static bool HasNotExtension(const String& path);

        static bool IsExist(const StringWide& path);
        static bool IsExist(const String& path);

        static String     GetCurrentDirectory();
        static StringWide GetCurrentDirectoryWide();

        static StringWide GetPathWide(const String& path);

        static String GetDirectory(const String& path);

        static size_t GetFileSize(const String& path);
    private:
    };
}
