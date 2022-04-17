/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Resource.hpp"

#include "../../Utility/FileUtility.hpp"

namespace Engine
{
    Resource::Resource(const String& path)
        : m_file_path(path)
    {
        m_extension = FileUtility::GetExtension(path);
        m_file_name = FileUtility::GetFileName(path);
        m_wide_path = FileUtility::GetPathWide(path);
    }

    bool Resource::IsLoaded() const
    {
        return m_b_loaded;
    }

    bool Resource::IsUnloaded() const
    {
        return m_b_unloaded;
    }

    String Resource::FileName() const
    {
        return m_file_name;
    }

    String Resource::FilePath() const
    {
        return m_file_path;
    }

    String Resource::FileExt() const
    {
        return m_extension;
    }

    eResourceType Resource::GetType() const
    {
        return m_type;
    }
}
