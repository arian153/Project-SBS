#include <filesystem>

#include "FileUtility.hpp"
#include "Utility.hpp"

namespace Engine
{
    String FileUtility::GetPath(const StringWide& path)
    {
        return std::filesystem::path(path).generic_string();
    }

    String FileUtility::GetExtension(const StringWide& path)
    {
        return std::filesystem::path(path).extension().generic_string();
    }

    String FileUtility::GetExtension(const String& path)
    {
        return std::filesystem::path(path).extension().generic_string();
    }

    String FileUtility::GetFileName(const StringWide& path)
    {
        return std::filesystem::path(path).filename().generic_string();
    }

    String FileUtility::GetFileName(const String& path)
    {
        return std::filesystem::path(path).filename().generic_string();
    }

    void FileUtility::GetFileListsInPath(const StringWide& path, std::vector<StringWide>& list)
    {
        if (IsExist(path))
        {
            for (auto& iterated_path : std::filesystem::directory_iterator(path))
            {
                list.emplace_back(iterated_path.path().generic_wstring());
            }
        }
    }

    void FileUtility::GetFileListsInPath(const StringWide& path, std::vector<String>& list)
    {
        if (IsExist(path))
        {
            for (auto& iterated_path : std::filesystem::directory_iterator(path))
            {
                list.emplace_back(iterated_path.path().generic_string());
            }
        }
    }

    void FileUtility::GetFileListsInPath(const String& path, std::vector<String>& list)
    {
        if (IsExist(path))
        {
            for (auto& iterated_path : std::filesystem::directory_iterator(path))
            {
                list.emplace_back(iterated_path.path().generic_string());
            }
        }
    }

    void FileUtility::GetFileListsInPathRecursive(const StringWide& path, std::vector<StringWide>& list)
    {
        for (auto& iterated_path : std::filesystem::recursive_directory_iterator(path))
        {
            list.emplace_back(iterated_path.path().generic_wstring());
        }
    }

    void FileUtility::GetFileListsInPathRecursive(const StringWide& path, std::vector<String>& list)
    {
        for (auto& iterated_path : std::filesystem::recursive_directory_iterator(path))
        {
            list.emplace_back(iterated_path.path().generic_string());
        }
    }

    void FileUtility::GetFileListsInPathRecursive(const String& path, std::vector<String>& list)
    {
        for (auto& iterated_path : std::filesystem::recursive_directory_iterator(path))
        {
            list.emplace_back(iterated_path.path().generic_string());
        }
    }

    bool FileUtility::IsDirectory(const StringWide& path)
    {
        return std::filesystem::is_directory(path);
    }

    bool FileUtility::IsDirectory(const String& path)
    {
        return std::filesystem::is_directory(path);
    }

    bool FileUtility::HasExtension(const String& path)
    {
        return std::filesystem::path(path).has_extension();
    }

    bool FileUtility::HasNotExtension(const String& path)
    {
        return !std::filesystem::path(path).has_extension();
    }

    bool FileUtility::IsExist(const StringWide& path)
    {
        return exists(std::filesystem::path(path));
    }

    bool FileUtility::IsExist(const String& path)
    {
        return exists(std::filesystem::path(path));
    }

    String FileUtility::GetCurrentDirectory()
    {
        return std::filesystem::current_path().generic_string();
    }

    StringWide FileUtility::GetCurrentDirectoryWide()
    {
        return std::filesystem::current_path().generic_wstring();
    }

    StringWide FileUtility::GetPathWide(const String& path)
    {
        return std::filesystem::path(path).generic_wstring();
    }
}
