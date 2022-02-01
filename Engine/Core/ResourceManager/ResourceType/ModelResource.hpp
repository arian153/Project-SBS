#pragma once
#include "../Resource.hpp"

namespace Engine
{
    class ModelResource final : public Resource
    {
    public:
        explicit ModelResource(const String& path);
        ~ModelResource() override;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();

    private:
        friend class ResourceManager;
    };
}
