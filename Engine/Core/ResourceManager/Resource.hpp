#pragma once
#include "ResourceType.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    class Resource
    {
    public:
        explicit Resource(const String& path);
        virtual  ~Resource() = default;

        bool IsLoaded() const;
        bool IsUnloaded() const;

        String FileName() const;
        String FilePath() const;
        String FileExt() const;

        eResourceType GetType() const;

    protected:
        virtual void Initialize() = 0;
        virtual void Shutdown() = 0;

    private:
        friend class ResourceManager;

    protected:
        StringWide m_wide_path;

        String m_file_path;
        String m_extension;
        String m_file_name;

        eResourceType m_type       = eResourceType::Undefined;
        bool          m_b_loaded   = false;
        bool          m_b_unloaded = false;
    };
}
